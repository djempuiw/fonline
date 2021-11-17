/*
 * Copyright 2015 MongoDB, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <bson/bson.h>
#include <mongoc/mongoc-util-private.h>

#include "mongoc/mongoc-client-private.h"

#include "mock-rs.h"
#include "sync-queue.h"
#include "../test-libmongoc.h"


struct _mock_rs_t {
   bool has_primary;
   int n_secondaries;
   int n_arbiters;
   int32_t max_wire_version;
   int64_t request_timeout_msec;

   mock_server_t *primary;
   mongoc_array_t secondaries;
   mongoc_array_t arbiters;
   mongoc_array_t servers;

   char *hosts_str;
   mongoc_uri_t *uri;
   sync_queue_t *q;

   bson_t primary_tags;
   bson_t **secondary_tags;
};


mock_server_t *
get_server (mongoc_array_t *servers, int i)
{
   return _mongoc_array_index (servers, mock_server_t *, i);
}


void
append_array (mongoc_array_t *dst, mongoc_array_t *src)
{
   _mongoc_array_append_vals (dst, src->data, (uint32_t) src->len);
}


/* a string like: "localhost:1","localhost:2","localhost:3" */
char *
hosts (mongoc_array_t *servers)
{
   int i;
   const char *host_and_port;
   bson_string_t *hosts_str = bson_string_new ("");

   for (i = 0; i < servers->len; i++) {
      host_and_port = mock_server_get_host_and_port (get_server (servers, i));
      bson_string_append_printf (hosts_str, "\"%s\"", host_and_port);

      if (i < servers->len - 1) {
         bson_string_append_printf (hosts_str, ", ");
      }
   }

   return bson_string_free (hosts_str, false); /* detach buffer */
}


mongoc_uri_t *
make_uri (mongoc_array_t *servers)
{
   int i;
   const char *host_and_port;
   bson_string_t *uri_str = bson_string_new ("mongodb://");
   mongoc_uri_t *uri;

   for (i = 0; i < servers->len; i++) {
      host_and_port = mock_server_get_host_and_port (get_server (servers, i));
      bson_string_append_printf (uri_str, "%s", host_and_port);

      if (i < servers->len - 1) {
         bson_string_append_printf (uri_str, ",");
      }
   }

   bson_string_append_printf (uri_str, "/?replicaSet=rs");

   uri = mongoc_uri_new (uri_str->str);

   bson_string_free (uri_str, true);

   return uri;
}


static char *
hello_json (mock_rs_t *rs,
               mongoc_server_description_type_t type,
               const bson_t *tags)
{
   char *server_type;
   char *mongos_36_fields = "";
   char *tags_json;
   char *hosts_str;
   char *json;

   if (type == MONGOC_SERVER_RS_PRIMARY) {
      server_type = "'isWritablePrimary': true, 'secondary': false, ";
   } else if (type == MONGOC_SERVER_RS_SECONDARY) {
      server_type = "'isWritablePrimary': false, 'secondary': true, ";
   } else {
      BSON_ASSERT (type == MONGOC_SERVER_RS_ARBITER);
      server_type = "'isWritablePrimary': false, 'arbiterOnly': true, ";
   }

   if (rs->max_wire_version >= WIRE_VERSION_OP_MSG) {
      mongos_36_fields =
         "'$clusterTime': {"
         "  'clusterTime': {'$timestamp': {'t': 1, 'i': 1}},"
         "  'signature': {"
         "    'hash': {'$binary': {'subType': '0', 'base64': ''}},"
         "    'keyId': {'$numberLong': '6446735049323708417'}"
         "  },"
         "  'operationTime': {'$timestamp': {'t': 1, 'i': 1}}"
         "}, "
         "'logicalSessionTimeoutMinutes': 30, ";
   }

   if (bson_empty0 (tags)) {
      tags_json = bson_strdup ("{}");
   } else {
      tags_json = bson_as_json (tags, NULL);
   }

   hosts_str = hosts (&rs->servers);

   json = bson_strdup_printf ("{'ok': 1, %s %s 'tags': %s,"
                              " 'maxWireVersion': %d, "
                              "'setName': 'rs', 'hosts': [%s]}",
                              server_type,
                              mongos_36_fields,
                              tags_json,
                              rs->max_wire_version,
                              hosts_str);

   bson_free (tags_json);
   bson_free (hosts_str);

   return json;
}


static char *
primary_json (mock_rs_t *rs)
{
   return hello_json (rs, MONGOC_SERVER_RS_PRIMARY, &rs->primary_tags);
}


static char *
secondary_json (mock_rs_t *rs, int server_number)
{
   return hello_json (
      rs, MONGOC_SERVER_RS_SECONDARY, rs->secondary_tags[server_number]);
}


static char *
arbiter_json (mock_rs_t *rs)
{
   return hello_json (rs, MONGOC_SERVER_RS_ARBITER, NULL);
}


/*--------------------------------------------------------------------------
 *
 * mock_rs_with_auto_hello --
 *
 *       A new mock replica set. Each member autoresponds to hello.
 *       Call mock_rs_run to start it, then mock_rs_get_uri to connect.
 *
 * Returns:
 *       A replica set you must mock_rs_destroy.
 *
 * Side effects:
 *       None.
 *
 *--------------------------------------------------------------------------
 */

mock_rs_t *
mock_rs_with_auto_hello (int32_t max_wire_version,
                         bool has_primary,
                         int n_secondaries,
                         int n_arbiters)
{
   int i;
   mock_rs_t *rs = (mock_rs_t *) bson_malloc0 (sizeof (mock_rs_t));

   rs->max_wire_version = max_wire_version;
   rs->has_primary = has_primary;
   rs->n_secondaries = n_secondaries;
   rs->n_arbiters = n_arbiters;
   rs->request_timeout_msec = get_future_timeout_ms ();
   rs->q = q_new ();
   bson_init (&rs->primary_tags);
   rs->secondary_tags = bson_malloc (n_secondaries * sizeof (bson_t *));

   for (i = 0; i < n_secondaries; i++) {
      rs->secondary_tags[i] = bson_new ();
   }

   return rs;
}


void
mock_rs_tag_primary (mock_rs_t *rs, const bson_t *tags)
{
   bson_destroy (&rs->primary_tags);
   bson_copy_to (tags, &rs->primary_tags);
}


void
mock_rs_tag_secondary (mock_rs_t *rs, int server_number, const bson_t *tags)
{
   bson_destroy (rs->secondary_tags[server_number]);
   rs->secondary_tags[server_number] = bson_copy (tags);
}


static void
mock_rs_auto_endsessions (mock_rs_t *rs)
{
   int i;

   for (i = 0; i < rs->servers.len; i++) {
      mock_server_auto_endsessions (get_server (&rs->servers, i));
   }
}


/*--------------------------------------------------------------------------
 *
 * mock_rs_get_request_timeout_msec --
 *
 *       How long mock_rs_receives_* functions wait for a client
 *       request before giving up and returning NULL.
 *
 *--------------------------------------------------------------------------
 */

int64_t
mock_rs_get_request_timeout_msec (mock_rs_t *rs)
{
   return rs->request_timeout_msec;
}

/*--------------------------------------------------------------------------
 *
 * mock_rs_set_request_timeout_msec --
 *
 *       How long mock_rs_receives_* functions wait for a client
 *       request before giving up and returning NULL.
 *
 *--------------------------------------------------------------------------
 */

void
mock_rs_set_request_timeout_msec (mock_rs_t *rs, int64_t request_timeout_msec)
{
   rs->request_timeout_msec = request_timeout_msec;
}


static bool
rs_q_append (request_t *request, void *data)
{
   mock_rs_t *rs = (mock_rs_t *) data;

   q_put (rs->q, (void *) request);

   return true; /* handled */
}


/*--------------------------------------------------------------------------
 *
 * mock_rs_run --
 *
 *       Start each member listening on an unused port. After this, call
 *       mock_rs_get_uri to connect.
 *
 * Returns:
 *       None.
 *
 * Side effects:
 *       The replica set's URI is set.
 *
 *--------------------------------------------------------------------------
 */

void
mock_rs_run (mock_rs_t *rs)
{
   int i;
   mock_server_t *server;
   char *hello;

   if (rs->has_primary) {
      /* start primary */
      rs->primary = mock_server_new ();
      mock_server_run (rs->primary);
   }

   /* start secondaries */
   _mongoc_array_init (&rs->secondaries, sizeof (mock_server_t *));

   for (i = 0; i < rs->n_secondaries; i++) {
      server = mock_server_new ();
      mock_server_run (server);
      _mongoc_array_append_val (&rs->secondaries, server);
   }

   /* start arbiters */
   _mongoc_array_init (&rs->arbiters, sizeof (mock_server_t *));

   for (i = 0; i < rs->n_arbiters; i++) {
      server = mock_server_new ();
      mock_server_run (server);
      _mongoc_array_append_val (&rs->arbiters, server);
   }

   /* add all servers to replica set */
   _mongoc_array_init (&rs->servers, sizeof (mock_server_t *));
   if (rs->has_primary) {
      _mongoc_array_append_val (&rs->servers, rs->primary);
   }

   append_array (&rs->servers, &rs->secondaries);
   append_array (&rs->servers, &rs->arbiters);

   /* enqueue unhandled requests in rs->q, they're retrieved with
    * mock_rs_receives_query() &co. rs_q_append is added first so it
    * runs last, after auto_hello.
    */
   for (i = 0; i < rs->servers.len; i++) {
      mock_server_autoresponds (
         get_server (&rs->servers, i), rs_q_append, (void *) rs, NULL);
   }


   /* now we know all servers' ports and we have them in one array */
   rs->hosts_str = hosts (&rs->servers);
   rs->uri = make_uri (&rs->servers);

   BSON_ASSERT (rs->max_wire_version > 0);
   if (rs->has_primary) {
      /* primary's hello response */
      hello = primary_json (rs);
      mock_server_auto_hello (rs->primary, hello);
      bson_free (hello);
   }

   /* secondaries' hello response */
   for (i = 0; i < rs->n_secondaries; i++) {
      hello = secondary_json (rs, i);
      mock_server_auto_hello (get_server (&rs->secondaries, i), hello);
      bson_free (hello);
   }

   /* arbiters' hello response */
   hello = arbiter_json (rs);

   for (i = 0; i < rs->n_arbiters; i++) {
      mock_server_auto_hello (get_server (&rs->arbiters, i), hello);
   }

   bson_free (hello);

   if (rs->max_wire_version >= WIRE_VERSION_OP_MSG) {
      mock_rs_auto_endsessions (rs);
   }
}


/*--------------------------------------------------------------------------
 *
 * mock_rs_get_uri --
 *
 *       Call after mock_rs_run to get the connection string.
 *
 * Returns:
 *       A const URI.
 *
 * Side effects:
 *       None.
 *
 *--------------------------------------------------------------------------
 */

const mongoc_uri_t *
mock_rs_get_uri (mock_rs_t *rs)
{
   return rs->uri;
}


/*--------------------------------------------------------------------------
 *
 * mock_rs_receives_request --
 *
 *       Pop a client request if one is enqueued, or wait up to
 *       request_timeout_ms for the client to send a request.
 *
 * Returns:
 *       A request you must request_destroy, or NULL if the request.
 *
 * Side effects:
 *       None.
 *
 *--------------------------------------------------------------------------
 */

request_t *
mock_rs_receives_request (mock_rs_t *rs)
{
   return (request_t *) q_get (rs->q, rs->request_timeout_msec);
}


/*--------------------------------------------------------------------------
 *
 * mock_rs_receives_query --
 *
 *       Pop a client request if one is enqueued, or wait up to
 *       request_timeout_ms for the client to send a request.
 *
 * Returns:
 *       A request you must request_destroy, or NULL if the request does
 *       not match.
 *
 * Side effects:
 *       Logs if the current request is not a query matching ns, flags,
 *       skip, n_return, query_json, and fields_json.
 *
 *--------------------------------------------------------------------------
 */

/* TODO: refactor with mock_server_receives_query, etc.? */
request_t *
mock_rs_receives_query (mock_rs_t *rs,
                        const char *ns,
                        mongoc_query_flags_t flags,
                        uint32_t skip,
                        int32_t n_return,
                        const char *query_json,
                        const char *fields_json)
{
   request_t *request;

   request = mock_rs_receives_request (rs);

   if (request &&
       !request_matches_query (
          request, ns, flags, skip, n_return, query_json, fields_json, false)) {
      request_destroy (request);
      return NULL;
   }

   return request;
}


/*--------------------------------------------------------------------------
 *
 * mock_server_reply_to_find --
 *
 *       Receive an OP_QUERY or a find command and reply to it.
 *
 *       Pop a client request if one is enqueued, or wait up to
 *       request_timeout_ms for the client to send a request.
 *
 * Side effects:
 *       Logs and aborts if the current request is not a query or find command
 *       matching "flags".
 *
 *--------------------------------------------------------------------------
 */
/*

void
mock_rs_reply_to_find (mock_rs_t           *rs,
                       mongoc_query_flags_t flags,
                       int64_t              cursor_id,
                       int32_t              number_returned,
                       const char          *reply_json,
                       bool                 is_command)
{
   request_t *request;

   request = mock_rs_receives_request (rs);
   BSON_ASSERT (request);

   mock_server_reply_to_find (request, flags, cursor_id, number_returned,
                              reply_json, is_command);
}
*/

/*--------------------------------------------------------------------------
 *
 * mock_rs_receives_command --
 *
 *       Pop a client request if one is enqueued, or wait up to
 *       request_timeout_ms for the client to send a request.
 *
 * Returns:
 *       A request you must request_destroy, or NULL if the request does
 *       not match.
 *
 * Side effects:
 *       Logs if the current request is not a command matching
 *       database_name, command_name, and command_json.
 *
 *--------------------------------------------------------------------------
 */

MONGOC_PRINTF_FORMAT (4, 5)
request_t *
mock_rs_receives_command (mock_rs_t *rs,
                          const char *database_name,
                          mongoc_query_flags_t flags,
                          const char *command_json,
                          ...)
{
   va_list args;
   char *formatted_command_json = NULL;
   char *ns;
   request_t *request;

   va_start (args, command_json);
   if (command_json) {
      formatted_command_json = bson_strdupv_printf (command_json, args);
   }
   va_end (args);

   ns = bson_strdup_printf ("%s.$cmd", database_name);

   request = (request_t *) q_get (rs->q, rs->request_timeout_msec);

   if (request &&
       !request_matches_query (
          request, ns, flags, 0, 1, formatted_command_json, NULL, true)) {
      bson_free (formatted_command_json);
      request_destroy (request);
      return NULL;
   }

   bson_free (ns);
   bson_free (formatted_command_json);

   return request;
}

/*--------------------------------------------------------------------------
 *
 * mock_rs_receives_insert --
 *
 *       Pop a client request if one is enqueued, or wait up to
 *       request_timeout_ms for the client to send a request.
 *
 * Returns:
 *       A request you must request_destroy, or NULL if the request does
 *       not match.
 *
 * Side effects:
 *       Logs if the current request is not an insert matching ns, flags,
 *       and doc_json.
 *
 *--------------------------------------------------------------------------
 */

request_t *
mock_rs_receives_insert (mock_rs_t *rs,
                         const char *ns,
                         mongoc_insert_flags_t flags,
                         const char *doc_json)
{
   request_t *request;

   request = (request_t *) q_get (rs->q, rs->request_timeout_msec);

   if (request && !request_matches_insert (request, ns, flags, doc_json)) {
      request_destroy (request);
      return NULL;
   }

   return request;
}


/*--------------------------------------------------------------------------
 *
 * mock_rs_receives_getmore --
 *
 *       Pop a client request if one is enqueued, or wait up to
 *       request_timeout_ms for the client to send a request.
 *
 * Returns:
 *       A request you must request_destroy, or NULL if the request does
 *       not match.
 *
 * Side effects:
 *       Logs if the current request is not a getmore matching n_return
 *       and cursor_id.
 *
 *--------------------------------------------------------------------------
 */

request_t *
mock_rs_receives_getmore (mock_rs_t *rs,
                          const char *ns,
                          int32_t n_return,
                          int64_t cursor_id)
{
   request_t *request;

   request = (request_t *) q_get (rs->q, rs->request_timeout_msec);

   if (request && !request_matches_getmore (request, ns, n_return, cursor_id)) {
      request_destroy (request);
      return NULL;
   }

   return request;
}


/*--------------------------------------------------------------------------
 *
 * mock_rs_receives_msg --
 *
 *       Pop a client OP_MSG request if one is enqueued, or wait up to
 *       request_timeout_ms for the client to send a request. Pass varargs
 *       list of bson_t pointers, which are matched to the series of
 *       documents in the request, regardless of section boundaries.
 *
 * Returns:
 *       A request you must request_destroy.
 *
 * Side effects:
 *       Logs and aborts if the current request is not an OP_MSG matching
 *       flags and documents.
 *
 *--------------------------------------------------------------------------
 */

request_t *
_mock_rs_receives_msg (mock_rs_t *rs, uint32_t flags, ...)
{
   request_t *request;
   va_list args;
   bool r;

   request = (request_t *) q_get (rs->q, rs->request_timeout_msec);

   va_start (args, flags);
   r = request_matches_msgv (request, flags, &args);
   va_end (args);

   if (!r) {
      request_destroy (request);
      return NULL;
   }

   return request;
}


/*--------------------------------------------------------------------------
 *
 * mock_rs_hangs_up --
 *
 *       Hang up on a client request.
 *
 * Returns:
 *       None.
 *
 * Side effects:
 *       Causes a network error on the client side.
 *
 *--------------------------------------------------------------------------
 */

void
mock_rs_hangs_up (request_t *request)
{
   mock_server_hangs_up (request);
}


/*--------------------------------------------------------------------------
 *
 * mock_rs_receives_kill_cursors --
 *
 *       Pop a client request if one is enqueued, or wait up to
 *       request_timeout_ms for the client to send a request.
 *
 *       Real-life OP_KILLCURSORS can take multiple ids, but that is
 *       not yet supported here.
 *
 * Returns:
 *       A request you must request_destroy, or NULL if the request
 *       does not match.
 *
 * Side effects:
 *       Logs if the current request is not an OP_KILLCURSORS with the
 *       expected cursor_id.
 *
 *--------------------------------------------------------------------------
 */

request_t *
mock_rs_receives_kill_cursors (mock_rs_t *rs, int64_t cursor_id)
{
   request_t *request;

   request = (request_t *) q_get (rs->q, rs->request_timeout_msec);

   if (request && !request_matches_kill_cursors (request, cursor_id)) {
      request_destroy (request);
      return NULL;
   }

   return request;
}


/*--------------------------------------------------------------------------
 *
 * mock_rs_replies --
 *
 *       Respond to a client request.
 *
 * Returns:
 *       None.
 *
 * Side effects:
 *       Sends an OP_REPLY to the client.
 *
 *--------------------------------------------------------------------------
 */

void
mock_rs_replies (request_t *request,
                 uint32_t flags,
                 int64_t cursor_id,
                 int32_t starting_from,
                 int32_t number_returned,
                 const char *docs_json)
{
   mock_server_replies (
      request, flags, cursor_id, starting_from, number_returned, docs_json);
}


static mongoc_server_description_type_t
_mock_rs_server_type (mock_rs_t *rs, uint16_t port)
{
   int i;

   if (rs->primary && port == mock_server_get_port (rs->primary)) {
      return MONGOC_SERVER_RS_PRIMARY;
   }

   for (i = 0; i < rs->secondaries.len; i++) {
      if (port == mock_server_get_port (get_server (&rs->secondaries, i))) {
         return MONGOC_SERVER_RS_SECONDARY;
      }
   }

   for (i = 0; i < rs->arbiters.len; i++) {
      if (port == mock_server_get_port (get_server (&rs->arbiters, i))) {
         return MONGOC_SERVER_RS_ARBITER;
      }
   }

   return MONGOC_SERVER_UNKNOWN;
}


/*--------------------------------------------------------------------------
 *
 * mock_rs_replies_simple --
 *
 *       Respond to a client request.
 *
 * Returns:
 *       None.
 *
 * Side effects:
 *       Sends an OP_REPLY to the client.
 *
 *--------------------------------------------------------------------------
 */

void
mock_rs_replies_simple (request_t *request, const char *docs_json)
{
   mock_rs_replies (request, 0, 0, 0, 1, docs_json);
}


/*--------------------------------------------------------------------------
 *
 * mock_rs_replies_to_find --
 *
 *       Receive an OP_QUERY or "find" command and reply appropriately.
 *
 * Returns:
 *       None.
 *
 * Side effects:
 *       Very roughly validates the query or "find" command or aborts.
 *       The intent is not to test the driver's query or find command
 *       implementation here, see _test_kill_cursors for example use.
 *
 *--------------------------------------------------------------------------
 */

void
mock_rs_replies_to_find (request_t *request,
                         mongoc_query_flags_t flags,
                         int64_t cursor_id,
                         int32_t number_returned,
                         const char *ns,
                         const char *reply_json,
                         bool is_command)
{
   mock_server_replies_to_find (
      request, flags, cursor_id, number_returned, ns, reply_json, is_command);
}


/*--------------------------------------------------------------------------
 *
 * mock_rs_request_is_to_primary --
 *
 *       Check that the request is non-NULL and sent to a
 *       primary in this replica set.
 *
 * Returns:
 *       True if so.
 *
 * Side effects:
 *       None.
 *
 *--------------------------------------------------------------------------
 */

bool
mock_rs_request_is_to_primary (mock_rs_t *rs, request_t *request)
{
   BSON_ASSERT (request);

   return MONGOC_SERVER_RS_PRIMARY ==
          _mock_rs_server_type (rs, request_get_server_port (request));
}


/*--------------------------------------------------------------------------
 *
 * mock_rs_request_is_to_secondary --
 *
 *       Check that the request is non-NULL and sent to a
 *       secondary in this replica set.
 *
 * Returns:
 *       True if so.
 *
 * Side effects:
 *       None.
 *
 *--------------------------------------------------------------------------
 */

bool
mock_rs_request_is_to_secondary (mock_rs_t *rs, request_t *request)
{
   BSON_ASSERT (request);

   return MONGOC_SERVER_RS_SECONDARY ==
          _mock_rs_server_type (rs, request_get_server_port (request));
}


/*--------------------------------------------------------------------------
 *
 * mock_rs_stepdown --
 *
 *       Change the primary to a secondary.
 *
 *--------------------------------------------------------------------------
 */

void
mock_rs_stepdown (mock_rs_t *rs)
{
   char *json;

   BSON_ASSERT (rs->primary);
   rs->n_secondaries++;
   rs->secondary_tags =
      bson_realloc (rs->secondary_tags, rs->n_secondaries * sizeof (bson_t *));

   rs->secondary_tags[rs->n_secondaries - 1] = bson_copy (&rs->primary_tags);
   bson_reinit (&rs->primary_tags);

   json = secondary_json (rs, rs->n_secondaries - 1);
   mock_server_auto_hello (rs->primary, json);
   bson_free (json);

   _mongoc_array_append_val (&rs->secondaries, rs->primary);
   rs->primary = NULL;
}


/*--------------------------------------------------------------------------
 *
 * mock_rs_elect --
 *
 *       Change a secondary to the primary.
 *
 *--------------------------------------------------------------------------
 */

void
mock_rs_elect (mock_rs_t *rs, int id)
{
   char *json;
   size_t i;
   mock_server_t **ptrs;

   BSON_ASSERT (!rs->primary);
   BSON_ASSERT (id >= 0);
   BSON_ASSERT (id < rs->secondaries.len);

   rs->primary = get_server (&rs->secondaries, id);

   /* as the secondary becomes primary, its tags come along */
   bson_destroy (&rs->primary_tags);
   BSON_ASSERT (bson_steal (&rs->primary_tags, rs->secondary_tags[id]));

   /* primary_json() uses the current primary_tags */
   json = primary_json (rs);
   mock_server_auto_hello (rs->primary, json);
   bson_free (json);

   ptrs = (mock_server_t **) rs->secondaries.data;

   for (i = (size_t) id + 1; i < rs->secondaries.len; i++) {
      ptrs[i - 1] = ptrs[i];
      rs->secondary_tags[i - 1] = rs->secondary_tags[i];
   }

   rs->secondaries.len--;
   rs->n_secondaries--;
}


/*--------------------------------------------------------------------------
 *
 * mock_rs_destroy --
 *
 *       Free a mock_rs_t.
 *
 * Returns:
 *       None.
 *
 * Side effects:
 *       Destroys each member mock_server_t, closes sockets, joins threads.
 *
 *--------------------------------------------------------------------------
 */

void
mock_rs_destroy (mock_rs_t *rs)
{
   int i;

   for (i = 0; i < rs->servers.len; i++) {
      mock_server_destroy (get_server (&rs->servers, i));
   }

   _mongoc_array_destroy (&rs->secondaries);
   _mongoc_array_destroy (&rs->arbiters);
   _mongoc_array_destroy (&rs->servers);

   bson_free (rs->hosts_str);
   mongoc_uri_destroy (rs->uri);
   q_destroy (rs->q);

   bson_destroy (&rs->primary_tags);
   for (i = 0; i < rs->n_secondaries; i++) {
      bson_destroy (rs->secondary_tags[i]);
   }

   bson_free (rs->secondary_tags);
   bson_free (rs);
}

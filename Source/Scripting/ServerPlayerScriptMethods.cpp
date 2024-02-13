//      __________        ___               ______            _
//     / ____/ __ \____  / (_)___  ___     / ____/___  ____ _(_)___  ___
//    / /_  / / / / __ \/ / / __ \/ _ \   / __/ / __ \/ __ `/ / __ \/ _ `
//   / __/ / /_/ / / / / / / / / /  __/  / /___/ / / / /_/ / / / / /  __/
//  /_/    \____/_/ /_/_/_/_/ /_/\___/  /_____/_/ /_/\__, /_/_/ /_/\___/
//                                                  /____/
// FOnline Engine
// https://fonline.ru
// https://github.com/cvet/fonline
//
// MIT License
//
// Copyright (c) 2006 - 2023, Anton Tsvetinskiy aka cvet <cvet@tut.by>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#include "Common.h"

#include "Server.h"

// ReSharper disable CppInconsistentNaming

///@ ExportMethod ExcludeInSingleplayer
[[maybe_unused]] Critter* Server_Player_CreateCritter(Player* self, hstring protoId)
{
    return self->GetEngine()->CreatePlayerCritter(self, protoId);
}

///@ ExportMethod ExcludeInSingleplayer
[[maybe_unused]] Critter* Server_Player_LoadCritter(Player* self, ident_t crId)
{
    return self->GetEngine()->LoadPlayerCritter(self, crId);
}

///@ ExportMethod ExcludeInSingleplayer
[[maybe_unused]] void Server_Player_UnloadCritter(Player* self, Critter* cr)
{
    UNUSED_VARIABLE(self);

    self->GetEngine()->UnloadPlayerCritter(cr);
}

///@ ExportMethod ExcludeInSingleplayer
[[maybe_unused]] void Server_Player_SwitchCritter(Player* self, Critter* cr)
{
    self->GetEngine()->SwitchPlayerCritter(self, cr);
}

///@ ExportMethod ExcludeInSingleplayer
[[maybe_unused]] void Server_Player_DestroyCritter(Player* self, ident_t crId)
{
    self->GetEngine()->GetEngine()->DestroyPlayerCritter(self, crId);
}

///# ...
///# return ...
///@ ExportMethod ExcludeInSingleplayer
[[maybe_unused]] Critter* Server_Player_GetOwnedCritter(Player* self)
{
    return self->GetOwnedCritter();
}

///# ...
///# return ...
///@ ExportMethod
[[maybe_unused]] int Server_Player_GetAccess(Player* self)
{
    return self->Access;
}

///# ...
///# param access ...
///# return ...
///@ ExportMethod
[[maybe_unused]] bool Server_Player_SetAccess(Player* self, int access)
{
    if (access < ACCESS_CLIENT || access > ACCESS_ADMIN) {
        throw ScriptException("Wrong access type");
    }

    if (access == self->Access) {
        return true;
    }

    string pass;
    const auto allow = self->GetEngine()->OnPlayerGetAccess.Fire(self, access, pass);
    if (allow) {
        self->Access = static_cast<uint8>(access);
    }

    return allow;
}

///# ...
///# param text ...
///@ ExportMethod
[[maybe_unused]] void Server_Player_Message(Player* self, string_view text)
{
    self->Send_Text(nullptr, text, SAY_NETMSG);
}

///# ...
///# param textMsg ...
///# param numStr ...
///@ ExportMethod
[[maybe_unused]] void Server_Player_Message(Player* self, TextPackName textPack, uint numStr)
{
    self->Send_TextMsg(nullptr, SAY_NETMSG, textPack, numStr);
}

///# ...
///# param textMsg ...
///# param numStr ...
///# param lexems ...
///@ ExportMethod
[[maybe_unused]] void Server_Player_Message(Player* self, TextPackName textPack, uint numStr, string_view lexems)
{
    self->Send_TextMsgLex(nullptr, SAY_NETMSG, textPack, numStr, lexems);
}

///# ...
///# return ...
///@ ExportMethod
[[maybe_unused]] bool Server_Player_IsWebConnected(Player* self)
{
    return self->Connection->IsWebConnection();
}

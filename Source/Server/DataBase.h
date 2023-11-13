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

#pragma once

#include "Common.h"

#include "AnyData.h"

DECLARE_EXCEPTION(DataBaseException);

class DataBaseImpl;

class DataBase
{
    friend auto ConnectToDataBase(string_view connection_info) -> DataBase;

public:
    using Collection = unordered_map<ident_t, AnyData::Document>;
    using Collections = unordered_map<string, Collection>;
    using RecordsState = unordered_map<string, unordered_set<ident_t>>;

    DataBase();
    DataBase(const DataBase&) = delete;
    DataBase(DataBase&&) noexcept;
    auto operator=(const DataBase&) = delete;
    auto operator=(DataBase&&) noexcept -> DataBase&;
    explicit operator bool() const;
    ~DataBase();

    [[nodiscard]] auto GetAllIds(string_view collection_name) const -> vector<ident_t>;
    [[nodiscard]] auto Get(string_view collection_name, ident_t id) const -> AnyData::Document;
    [[nodiscard]] auto Valid(string_view collection_name, ident_t id) const -> bool;

    void Insert(string_view collection_name, ident_t id, const AnyData::Document& doc);
    void Update(string_view collection_name, ident_t id, string_view key, const AnyData::Value& value);
    void Delete(string_view collection_name, ident_t id);
    void CommitChanges(bool wait_commit_complete);

private:
    explicit DataBase(DataBaseImpl* impl);
    unique_ptr<DataBaseImpl> _impl {};
    bool _nonConstHelper {};
};

extern auto ConnectToDataBase(string_view connection_info) -> DataBase;

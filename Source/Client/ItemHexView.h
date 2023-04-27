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
// Copyright (c) 2006 - 2022, Anton Tsvetinskiy aka cvet <cvet@tut.by>
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

#include "HexView.h"
#include "ItemView.h"
#include "ResourceManager.h"

class ItemHexView final : public ItemView, public HexView
{
public:
    ItemHexView() = delete;
    ItemHexView(MapView* map, ident_t id, const ProtoItem* proto, const Properties* props = nullptr);
    ItemHexView(const ItemHexView&) = delete;
    ItemHexView(ItemHexView&&) noexcept = delete;
    auto operator=(const ItemHexView&) = delete;
    auto operator=(ItemHexView&&) noexcept = delete;
    ~ItemHexView() override = default;

    [[nodiscard]] auto IsDrawContour() const -> bool { return !GetIsWall() && !GetIsScenery() && !GetIsNoHighlight() && !GetIsBadItem(); }
    [[nodiscard]] auto GetEggType() const -> EggAppearenceType;
    [[nodiscard]] auto IsNeedProcess() const -> bool { return _begFrm != _endFrm || (_isEffect && !IsFinishing()) || _isAnimLooped || (_isDynamicEffect && !IsFinishing()) || IsFading(); }

    void Init();
    void Process();
    void RefreshAlpha();
    void RefreshAnim();
    void SetFlyEffect(uint16 to_hx, uint16 to_hy);
    void PlayAnimFromEnd();
    void PlayAnimFromStart();
    void PlayAnim(uint beg, uint end);
    void PlayStayAnim();
    void PlayShowAnim();
    void PlayHideAnim();
    void RefreshOffs();

private:
    void SetupSprite(MapSprite* spr) override;
    void SetCurSpr(uint num_spr);

    AnyFrames* _anim {};
    uint _curFrm {};
    uint _begFrm {};
    uint _endFrm {};
    uint _animBegFrm {};
    uint _animEndFrm {};
    time_point _animTime {};
    bool _isAnimLooped {};
    time_point _animStartTime {};

    bool _isEffect {};
    bool _isDynamicEffect {};
    float _effSx {};
    float _effSy {};
    int _effStartX {};
    int _effStartY {};
    float _effCurX {};
    float _effCurY {};
    uint _effDist {};
    time_point _effUpdateLastTime {};
    int _effDir {};
    vector<pair<uint16, uint16>> _effSteps {};
};

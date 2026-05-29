"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const UE = require("ue");
const puerts_1 = require("puerts");
// 获取蓝图类
let ucls = UE.Class.Load('/Game/Blueprints/BP_TestPuerTS.BP_TestPuerTS_C');
// 将蓝图类转换为 JS 类
const BP_Mixin = puerts_1.blueprint.tojs(ucls);
;
// 实现扩展类
class BPExt {
    // 重写 BeginPlay 函数
    ReceiveBeginPlay() {
        console.log('BPExt ReceiveBeginPlay() Called');
    }
}
// 混合蓝图类和扩展类
puerts_1.blueprint.mixin(BP_Mixin, BPExt);
//# sourceMappingURL=TestPuerTS.js.map
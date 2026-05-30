import * as UE from "ue";
import { blueprint } from "puerts";

// 获取蓝图类
let ucls = UE.Class.Load("/Game/Blueprints/BP_TestPuerTS.BP_TestPuerTS_C");

// 将蓝图类转换为 JS 类
const BP_Mixin = blueprint.tojs<typeof UE.Game.Blueprints.BP_TestPuerTS.BP_TestPuerTS_C>(ucls);

// 定义扩展类
interface BPExt extends UE.Game.Blueprints.BP_TestPuerTS.BP_TestPuerTS_C {}

// 实现扩展类
class BPExt {
    // 重写 BeginPlay 函数，注意：这个事件必须在蓝图中定义了，否则无法覆盖重写
    ReceiveBeginPlay() {
        console.log("BPExt ReceiveBeginPlay() Called");
    }
}

// 混合蓝图类和扩展类
blueprint.mixin(BP_Mixin, BPExt);

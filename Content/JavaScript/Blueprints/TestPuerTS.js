"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const UE = require("ue");
const puerts_1 = require("puerts");
let ucls = UE.Class.Load('/Game/Blueprints/BP_TestPuerTS.BP_TestPuerTS_C');
const BP_Mixin = puerts_1.blueprint.tojs(ucls);
;
class BPExt {
    ReceiveBeginPlay() {
        console.log('BPExt ReceiveBeginPlay() Called');
    }
}
puerts_1.blueprint.mixin(BP_Mixin, BPExt);
//# sourceMappingURL=TestPuerTS.js.map
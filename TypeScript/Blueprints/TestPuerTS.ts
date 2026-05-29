import * as UE from 'ue'
import { blueprint } from 'puerts';

let ucls = UE.Class.Load('/Game/Blueprints/BP_TestPuerTS.BP_TestPuerTS_C');

const BP_Mixin = blueprint.tojs<typeof UE.Game.Blueprints.BP_TestPuerTS.BP_TestPuerTS_C>(ucls);

interface BPExt extends UE.Game.Blueprints.BP_TestPuerTS.BP_TestPuerTS_C {};

class BPExt {
    ReceiveBeginPlay() {
        console.log('BPExt ReceiveBeginPlay() Called');
    }
}

blueprint.mixin(BP_Mixin, BPExt);
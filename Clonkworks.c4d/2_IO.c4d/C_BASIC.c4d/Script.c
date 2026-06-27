/*-- Neues Script --*/

#strict 2
func Hit(){ if(!Random(3)) Sound("MetalHit*"); }
func GetResearchBase(){ return(WRKT); }
func IsAnvilProduct(){ return(1); }

public func UsedCircuits(){ return([I1_B,I1_S,I1_K,I2_L,I2_T,I2_S,I1_T,I1_A,I1NP,I1SP,I1MN,I1TG,I2IS,I2HL]); }

public func Activate(pClonk){
    [$TxtUse$]
    CreateMenu(GetID(),pClonk,this,4,"$TxtMenu$");
    for(var Thing in UsedCircuits()){
        AddMenuItem("%s","NewCircuit",Thing,pClonk,0,0,GetDesc(,Thing),6,GetPlrColorDw(GetOwner(Contained())));
    }

    return(1);
}

public func NewCircuit(Type){
    var yOffset = 10;
    if(DefinitionCall(Type,"MustBeOnWall")){
        yOffset = 3;
        if((GBackSolid() || GBackLiquid() || GBackSky()) && !FindCastleWall()){
            Message("$TxtNoWall$",Contained());
            Sound("Error");
            return(0);
        }
    }

    if(FindObject2(Find_Func("IsCircuit"),Find_Distance(10),Find_NoContainer()) || FindObject2(Find_Category(C4D_Structure),Find_AtPoint(0,yOffset),Find_NoContainer())){
        Message("$TxtSpace$",Contained());
        Sound("Error");
         return(0);
    }

    Message("$TxtPlaced$",Contained(),GetName(,Type));
    Sound("Connect");
    var circuit = CreateConstruction(Type,0,yOffset,GetOwner(Contained()),100);
    if(FindCastleWall()) circuit->StartDependingOn(FindCastleWall());
    RemoveObject();
}

func FindCastleWall(){
    var OnTop = FindObjects(Find_Category(C4D_StaticBack),Find_OCF(OCF_Fullcon),Find_NoContainer(),Find_AtPoint());
    for(var Part in OnTop){
        if(Part->~CastlePartWidth()) return(Part);
    }
}
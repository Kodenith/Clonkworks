/*-- Neues Script --*/

#strict

func Hit () {
  if(!Random(3)) Sound("MetalHit*");
}

func IsAnvilProduct(){ return(1); }
func IsAdvancedProduct(){ return(1); }
public func GetResearchBase(){ return(CNVY); }

//Auto Cannon Tower
func Entrance(pContainer){
  if(GetID(pContainer) == CTW0){
    var SavedAction = GetAction(pContainer);
    var SavedPhase = GetPhase(pContainer);

    ChangeDef(ACTW,pContainer);
    pContainer->Sound("Connect");
    pContainer->SetAction(SavedAction);
    pContainer->SetPhase(SavedPhase);
    
    RemoveObject();
  }
}
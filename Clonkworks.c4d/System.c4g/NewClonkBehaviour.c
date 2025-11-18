/*-- New Behaviour --*/
//new behaviour that plans to fix some dumb pathfinding decissions.

#strict 2
#appendto CLNK

//grab currently uncraftable packable items. this makes getting honey without any weird glitches possible.
public func ControlCommandAcquire(target, x, y, target2, def)
{
  if(DefinitionCall(def,"PackTo") && !GetProducerOf(def) && !FindObject(def)){
	if(GetAvailableObject (def, target2)) return(_inherited(target,x,y,target2,def));
	DebugLog("Finding packed %v.",def);
	var obj = FindPacked(def);
	if(!obj) return(_inherited(target,x,y,target2,def));
	//if(GetAvailableObject (obj, target2)) return(_inherited(target,x,y,target2,def));
	AddCommand(this(),"Call",this(),def, , ,0,"GrabArrow");
	AddCommand(this(),"Acquire", , , , ,0,obj);
	
	return(1);
  }
  return(_inherited(target,x,y,target2,def));
}

//arrowpack stuff

public func FindPacked(def){
	var fdef, i;
	while(fdef = GetDefinition(i++,C4D_Object)){
		if(DefinitionCall(fdef,"UnpackTo") == def){
			return fdef;
		}
	}
	
	return 0;
}

public func GrabArrow(pClonk,type){
	if(!FindPacked(type)) return 0;
	var Item = FindObject2(Find_Container(pClonk),Find_ID(FindPacked(type)));
	if(!Item) return 0;
	Item->GetItem();
	if(Item){
	if(ContainedTop(pClonk)) Enter(ContainedTop(pClonk),Item);
	else Exit(Item);
	}
	return(1);
}
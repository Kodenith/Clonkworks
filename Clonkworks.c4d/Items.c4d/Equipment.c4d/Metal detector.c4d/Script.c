/*-- Neues Script --*/

#strict 2
local Search;

func Initialize() {
  AddEffect("MTLDLogic",this(),1,1,this());
  return(1);
}

func Hit(){
	if(!Random(3)) Sound("MetalHit*");
}

public func Activate(pClonk){
	if(!Search){
		CreateMenu(GetID(),pClonk,this(),0,"$MenuEmpty$");
		var i = 0;
		var def = 0;
		while(def = GetDefinition(i)){
			if(DefinitionCall(def,"MTLDMetal") && ObjectCount(def))
				AddMenuItem("$MenuLookFor$","StartSearch",def,pClonk,ObjectCount(def));
			i++;
		}
		return(1);
	}else{
		Message("$MsgStop$",ContainedTop());
		Sound("Click");
		Search = 0;
		return(1);
	}
}

public func StartSearch(def){
	if(ContainedTop())
		Message("$MsgLook$",ContainedTop(),GetName(,def));
	Search = def;
	Sound("Ding");
}

//Effect
func FxMTLDLogicInfo(object pTarget, int iEffectNumber){
	if(Search) return(Format("$InfoOn$",Search,GetName(,Search)));
	return("$InfoOff$");
}

func FxMTLDLogicTimer(object pTarget, int iEffectNumber, int iEffectTime){
	if(!Search) return(0);
	
	var Find = FindObject2(Find_ID(Search), Find_NoContainer(), Sort_Distance());
	if(!Find) Find = FindObject2(Find_ID(Search),Sort_Distance());
	if(!Find){
		Sound("Error");
		if(ContainedTop()) Message("$MsgNoMore$",ContainedTop(),GetName(,Search));
		Search = 0;
		return(0);
	}
	
	var Distance = ObjectDistance(Find);
	Distance /= 16;
	if(Distance == 0) Distance = 1;
	if(Distance > 36*3) Distance = 36*3;
	
	if(iEffectTime%Distance == 0){
		Sound("DetectBeep");
		var RotDef = Angle(GetX(),GetY(),GetX(Find),GetY(Find));
		var pXDir = Sin(RotDef, 15);
		var pYDir = -Cos(RotDef, 15);
		if(Distance > 8)
		CreateParticle("PSpark",0,0,pXDir,pYDir,RandomX(20,35),GetColorDw(),ContainedTop());
	}
}

//public funcs
public func IsAnvilProduct(){ return(1); }
public func IsAdvancedProduct(){ return(1); }
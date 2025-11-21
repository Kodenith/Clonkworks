/*-- Neues Script --*/

#strict 2

func Initialize() {
  AddEffect("PCANBio",this(),1,0,this());
  SetCategory(C4D_Object);
  return(1);
}

public func Activate(pClonk){
	[$TxtUse$]
	if(Contained() != pClonk) return(1);
	if(ContentsCount() > 1){
		if(GetEffectCount("*Cooldown*",this())) return(0);
		ChangeDef(PCN2); // placeholder
		SetCategory(C4D_Vehicle);
		pClonk->SetAction("Push",this());
		Exit();
		this()->SetAction("NoAim");
		SetPosition(GetX(pClonk),GetY(pClonk),this());
		if(GetDir(pClonk) == DIR_Left){
			SetR(-90);
			this()->SetDir(DIR_Left);
		}else{
			SetR(90);
			this()->SetDir(DIR_Right);
		}
	}else{
		if(!ContentsCount(GUNP)){
			var gunp = FindObject2(Find_ID(GUNP),Find_Distance(10),Find_NoContainer(),Sort_Distance());
			if(!gunp){
				Message("$TxtReq1$",this());
				pClonk->Sound("CommandFailure1");
				return(1);
			}
			Enter(this(),gunp);
			Sound("Click");
		}else{
			var item = FindObject2(Find_Category(C4D_Object),Find_Not(Find_ID(GUNP)),Find_Distance(10),Find_NoContainer(),Sort_Distance());
			if(!item){
				Message("$TxtReq2$",this());
				pClonk->Sound("CommandFailure1");
				return(1);
			}
			Enter(this(),item);
			Message(Format("$TxtLoad$",GetID(item),GetName(item)),this());
			Sound("Click");
		}
	}
	return(1);
}

public func ContextEmpty(pClonk){
	[$TxtEmpty$|Image=ADM1]
	if(Contained() != pClonk) return(1);
	while(Contents(0)){
		Exit(Contents(0),0,5);
	}
}

func Hit(){ if(!Random(3)) Sound("WoodHit*"); }
func IsAnvilProduct(){ return(1);}
//func IsAdvancedProduct(){ return(1);}

//EFFECTS

//Contents information
func FxPCANBioInfo(object pTarget, int iEffectNumber){
	if(GetID() != PCAN) return(0);
	if(FindOtherContents(GUNP)){
		var i = FindOtherContents(GUNP);
		return(Format("$TxtLoad$",GetID(i),GetName(i)));
	}
}

//Cooldown, used in functions to prevent shooting.
func FxPCANCooldownTimer(object pTarget, int iEffectNumber, int iEffectTime){
	if(iEffectTime > 36*2) return(-1);
	if(!Random(3)) pTarget->Smoke(0,0,RandomX(5,10));
}

//Cooldown applying function
public func ApplyCooldown(){
	AddEffect("PCANCooldown",this(),100,5,this());
}
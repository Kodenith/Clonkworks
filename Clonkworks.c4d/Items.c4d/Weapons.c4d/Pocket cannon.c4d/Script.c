/*-- Neues Script --*/

#strict 2

func Initialize() {
  SetCategory(C4D_Object);
  return(1);
}

public func Activate(pClonk){
	[$TxtUse$]
	if(Contained() != pClonk) return(1);
	if(ContentsCount() > 1){
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
func IsAdvancedProduct(){ return(1);}



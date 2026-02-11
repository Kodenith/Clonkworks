/*-- Neues Script --*/

#strict 2
local Owner;
local sX;
local MiningMode;

func ControlDigDouble(pClonk){
		[$TxtDown$|Image=EXDC:2]
	if(FindObject2(Find_Category(C4D_Structure),Find_NoContainer(),Find_AtPoint())){
		Message("$TxtWarning$",this);
		Sound("Error");
		return(0);
	}

	var grabber;
	while( grabber = FindObject2(Find_Action("Push"),Find_ActionTarget(this)) ){
		grabber->SetAction("Walk");
	}

	SetY(GetY()-15);
	SetAction("Begin");
	SetComDir(COMD_Stop);
	Sound("Connect");
	sX=GetX();
	SetVertex(4,2,8,this,2);
	SetXDir(0);
	SetYDir(0);
}

func RejectGrabbed(pObj){
	if(!ActIdle()) return(1);
}

func DoDrilling(){
	SetComDir(COMD_Down);
	SetX(sX);
	if(!MiningMode && GetActTime()>(38*7)){
		Explode(30);
	}else if(MiningMode && GetActTime()>(38*20)){
		Explode(30);
	}

	if(GetActTime()%2) return(0);
	if(GetActTime()%3 && !MiningMode) return(0);
	if(GetContact(this(),-1) & CNAT_Bottom){
		if(GetMaterial(0,GetVertex(4,1)+2) == Material("Vehicle") && FindObject2(Find_Func("IsOreDeposit"),Find_AtPoint(0,GetVertex(4,1)))){
			MiningMode = 1;
			return(MineDeposit(FindObject2(Find_Func("IsOreDeposit"),Find_AtPoint(0,GetVertex(4,1)))));
		}else{
			MiningMode = 0;
		}
		var X = GetVertex(4,0);
		var Y = GetVertex(4,1);
		Y += RandomX(-2,2);
		X += RandomX(-8,8);
		
		var Explo = CreateObject(FLNT,X,Y);
		Explo->Explode(RandomX(10,15));
	}
}

func MineDeposit(pDep){
	var X = GetVertex(4,0);
	var Y = GetVertex(4,1);
	Y += RandomX(-2,2);
	X += RandomX(-8,8);

	CastParticles("PxSpark",5,100,X,Y,10,50,RGBa(255,255,0),RGBa(255,255,0));
    if(pDep->~GetMined(this())){
		CreateParticle("Blast", X,Y, 0,0, 100, RGBa(255,255,255,0));
	}
}

func RejectCollect(pId,pObj){
	if(GetAction() == "Drilling"){
		SetYDir(RandomX(-50,-80),pObj);
		SetXDir(RandomX(-30,30),pObj);
	}

	return(1);
}

func IsAdvancedProduct(){ return(1); }
func GetResearchBase(){ return(EXDR); }
/*-- Neues Script --*/

#strict 3

local IsOn;

func Initialize() {
  IsOn = false;
  SetAction("Aim");
  return(1);
}

public func IsHoseOn(){ return(false); }

public func Spew(int Amount, string Mat, int minPower, int maxPower){
	var angle;
	
	if(Contained()) SetDir(GetDir(Contained()));
	
	 if(GetPhase() == 2){ //Aim Horizontally
		 angle = -25;
		 if(GetDir() == DIR_Left) angle = 205;
	 }
	 
	 if(GetPhase() == 1){ //Aim Vertically
		 angle = -45;
		 if(GetDir() == DIR_Left) angle = -135;
	 }
	 
	 if(GetPhase() == 0) angle = -90; //aim up
	 
	 for(var i = 0; i < Amount; i++){
		 CastPXSX(Mat, 1, RandomX(minPower,maxPower),0,-3,angle);
	 }
}

protected func Activate(pClonk){
	[$Togl$|Image=L_RL]
	var line;
	var ovrlp = FindObject2(Find_NoContainer(), Find_OCF(OCF_LineConstruct), Find_AtPoint());
	
	for(var i in FindObjects(Find_Action("Connect"), Find_Func("isLine"))){
		if(GetActionTarget(1, i) == this() && GetActionTarget(0, i) == ovrlp){
			line = i;
			break;
		}
	}
	
	if(!line) return(Toggle(pClonk));
	
	Sound("Connect");
	RemoveObject(line);
	CreateObject(FNKT);
	RemoveObject(this());
	
	return(1);
}

func Toggle(pClonk){
	Sound("Click");
	ChangeDef(HSH2);
	Exit();
	pClonk->SetAction("Push",this());
	SetCategory(C4D_Vehicle);
			SetPosition(GetX(pClonk),GetY(pClonk),this());
		if(GetDir(pClonk) == DIR_Left){
			SetR(-90);
			this()->SetDir(DIR_Left);
		}else{
			SetR(90);
			this()->SetDir(DIR_Right);
		}
	return(1);
}

public func RequiresLine(){ return(true); } //line attached to hose must be disconnected via the hose!
/*-- Neues Script --*/

#strict 2
local Durability;
local Owner;

func Initialize(){
	Durability = 0;
	SetAction("NoHead");
	PutHead();
}

func Timer(){
	if(!Owner) RemoveObject();
	if(GetY() < GetY(Owner)){
		SetY(GetY(Owner));
		SetComDir(COMD_Stop);
	}
	if(GetComDir() != COMD_Stop){
		Sound("Elevator",0,this(),0,0,+1);
	}else{
		Sound("Elevator",0,this(),0,0,-1);
		SetYDir(0);
	}
	
	if(GetAction() == "Idle"){
		if(Durability > 0) SetAction("DrillIdle");
		else SetAction("NoHead");
	}
	
	if(GetAction() != "NoHead" && Durability <= 0){
		Durability = 0;
		SetAction("NoHead");
		Sound("Discharge");
		SetComDir(COMD_Up);
		
		var debri = CreateObject(EXDH,0,10);
		SetR(180,debri);
		DoDamage(1000,debri);
		return(1);
	}
	
	if(!EnergyCheck(1,Owner)){
		SetComDir(COMD_Stop);
	}
	
	if(GetYDir() > 0 && GetAction() == "DrillIdle") SetAction("Drilling");
	if((GetYDir() < 0 || GetComDir() == COMD_Stop) && GetAction() == "Drilling") SetAction("DrillIdle");
	
	var head;
	if(GetAction() == "NoHead" && FindObject2(Find_ID(EXDH),Find_NoContainer(),Find_AtPoint(GetVertex(4,0),GetVertex(4,1)))){
		head=FindObject2(Find_ID(EXDH),Find_NoContainer(),Find_AtPoint(GetVertex(4,0),GetVertex(4,1)));
		if(GetAction(head) == "Broken") return(0);
		RemoveObject(head);
		PutHead();
	}
}

func PutHead(){
	Durability = 500;
	Sound("Connect");
	SetAction("DrillIdle");
}

func DoDrilling(){
	if(GetActTime()%2 || GetActTime()%3) return(0);
	if(GetContact(this(),-1) & CNAT_Bottom){
		var X = GetVertex(4,0);
		var Y = GetVertex(4,1);
		Y += RandomX(-2,2);
		X += RandomX(-8,8);
		
		var Explo = CreateObject(FLNT,X,Y);
		Explo->Explode(RandomX(10,15));
		Durability -= RandomX(0,20);
	}
}

func Destruction(){
    if(GetAction() != "NoHead"){
		var head = CreateObject(EXDH);
	}
}

func Collection(pObj){
	Enter(Owner,pObj);
}
/*-- Neues Objekt --*/
//this track moves along the verticies of a line.
#strict 2

local TargetVert; //currently moving towards
local Line; //the line.
local Step; //step, mnax 50, then sets root pos
local RootX,RootY; //where to go from?
local memorizedCount; //memorized amount of verts
local Dragged; //used by the pole.

local SoundPlaying; //check if sound is playing

func Initialize(){
	RootX = GetX();
	RootY = GetY();
}

func Move(){
	SetXDir(0); SetYDir(0);
	if(TargetVert == 0) TargetVert = 1;
	if(GetVertexNum(Line) < TargetVert) Finish();
	if(memorizedCount != GetVertexNum(Line)){
		Finish();
		return(0);
	}
	var ang = Angle(RootX,RootY,GetVertex(TargetVert,0,Line),GetVertex(TargetVert,1,Line));
	ang = (ang + 180) % 360 - 180;
	
	var ix,iy;
	ix = Sin(ang, Step);
	iy = -Cos(ang,Step);
	
	//both poles need power
	var Pole1, Pole2;
	Pole1 = GetActionTarget(0,Line);
	Pole2 = GetActionTarget(1,Line);
	if(!Pole1 || !Pole2 || OnFire(Pole1) || OnFire(Pole2)) Finish();
	if(!EnergyCheck(200,Pole1) || !EnergyCheck(200,Pole2)){
		if(!Random(3)) DoDamage(1);
		ShutUp();
		return(0);
	}
	
	if(GetID(Pole1) == FNKT || GetID(Pole2) == FNKT){
		Finish();
	}
	
	if(GBackSolid(AbsX(RootX)+ix,AbsY(RootY)+iy)){
		if(!Random(3)) DoDamage(1);
		ShutUp();
		return(0);
	}
	
	DoEnergy(-1,Pole1);
	DoEnergy(-1,Pole2);
	
	SetX(RootX+ix);
	SetY(RootY+iy);
	Step++;
	
	if(Step%50 == 0){
		RootX = GetX();
		RootY = GetY();
		Step = 0;
	}
	
	if(!SoundPlaying){
		SoundPlaying = true;
		Sound("Elevator",false,this(),75,0,+1,,650);
	}
	
	if(Distance(GetX(),GetY(),GetX(Line)+GetVertex(TargetVert,0,Line),GetY(Line)+GetVertex(TargetVert,1,Line)) < 10){
		RootX = GetVertex(TargetVert,0,Line);
		RootY = GetVertex(TargetVert,1,Line);
		TargetVert++;
		Step = 0;
	}
}

func Finish(){
	Sound("Connect");
	var rope = CreateObject(CK5P);
	var pole;
	if(pole = FindObject2(Find_ID(ZIPP),Find_AtPoint())){
		Enter(pole,rope);
	}
	RemoveObject();
}

func Damage(){
	if(GetDamage() > 200) Finish();
}

func ShutUp(){
	if(SoundPlaying){
		SoundPlaying = false;
		Sound("Elevator",false,this(),75,0,-1,,650);
	}
}
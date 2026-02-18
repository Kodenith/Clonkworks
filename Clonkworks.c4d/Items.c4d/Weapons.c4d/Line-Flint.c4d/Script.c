/*-- Neues Script --*/

#strict 2
#include TFLS
local LookAt;

public func Activate(pClonk)
{
 if(!ActIdle()) return true;
 LookAt = pClonk;
 
 // Attaching to ceilings and walls
 if(GetProcedure(pClonk) == "SCALE"){
	 var steps = 10; //max steps
	 var dir = GetDir2(pClonk);
	 Exit();
	 while(steps > 0 && !GBackSolid()){
		 SetX(GetX()+dir);
		 steps--;
	 }
	 SetX(GetX()+dir);
 }
 
 if(GetProcedure(pClonk) == "HANGLE"){
	 var steps = 10; //max steps
	 Exit();
	 while(steps > 0 && !GBackSolid()){
		 SetY(GetY()-1);
		 steps--;
	 }
	 SetY(GetY()-1);
 }
 
 Exit(this(), 0, (GetDefBottom(pClonk)-GetY())+2);
 return(Attachment());
}

public func Attachment(){
   SetPicture(0, 7 + 64, 64, 64);
   Sound("Fuse");
   SetAction("Active");
}

public func Stickiness(){
	if(!(GetContact(this(),-1) & 8) && GetAction() == "Active"){
		Sound("Pshshsh");
		SetActionKeepPhase("Idle");
	}
	
	if(GetAction() == "Active" && LookAt){
		SetR(Angle(GetX(), GetY(),
           GetX(LookAt), GetY(LookAt) ) + 90);
		   
		if(GetActTime() == 120-18) Sound("Minebeep");
		var vert = RandomX(1,8);
		if(!Random(3)) CreateParticle("PSpark",GetVertex(vert,0),GetVertex(vert,1),0,0,25,RGBa(255,0,0),this());
	}
}

public func Departure(pClonk){
	LookAt = pClonk;
}

public func ExplodeSize() { return(20); }


public func DoExplosion(){
	SetXDir(0); SetYDir(0); SetRDir(0);
	SetR(GetR());
	CreateChainExplosion(0);
	Sound("Drill");
	Schedule("CreateChainExplosion(1,1)",5);
	Schedule("CreateChainExplosion(5,1)",5);
	
	Schedule("CreateChainExplosion(2,1)",10);
	Schedule("CreateChainExplosion(6,1)",10);
	
	Schedule("CreateChainExplosion(3)",15);
	Schedule("CreateChainExplosion(7)",15);
/* 	
	Schedule("CreateChainExplosion(4)",20);
	Schedule("CreateChainExplosion(8)",20); */
	
	Schedule("RemoveObject()",16);
}

public func CreateChainExplosion(vert,fal){
	if(!fal)
	CreateObject(FLNT,GetVertex(vert,0),GetVertex(vert,1))->Explode(ExplodeSize());
	else{
	var obj = CreateObject(FLNT,GetVertex(vert,0),GetVertex(vert,1));
	TerrainExplosion(ExplodeSize(),obj);
	}
}

func TerrainExplosion(size,obj){
	obj->CreateParticle("Blast",0,0,0,0,size*10,RGBa(255,255,255,200));
	obj->Sound("Blast1");
	obj->BlastFree(0,0,size);
	RemoveObject(obj);
}

protected func Hit(xdir, ydir) {
  if(!ActIdle()) return true;
  var iX, iY;
  if(GetXDir() < 0) iX = -1;
  if(GetXDir() > 0) iX = 1;
  if(ydir < 0) iY = -2;
  SetPosition(GetX()+iX,GetY()+iY);
	return(Attachment());
}

func FixPic(){
	SetPicture(0, 0, 64, 64);
}

func RejectEntrance(){
	if(GetAction() != "Idle") return(1);
}
public func IsAdvancedProduct(){ return(1); }
public func Malfunction(){
	if(GetAction() != "Active") return(0);
	CreateObject(GetID());
	Sound("Click");
	RemoveObject();
}

public func IsAdvancedProduct(){ return(1); }
public func IsAnvilProduct(){ return(1); }
public func IsChemicalProduct(){ return(0); }
public func GetResearchBase(){ return(MINE); }
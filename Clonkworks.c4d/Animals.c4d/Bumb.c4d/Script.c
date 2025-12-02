#strict 2
#include ANIM

//Movement Goals
local gX;
local gY;
local Walkdir;

local MovingHome;

local Beenergy;
local MaxBeenergy;

local GrudgeTarget;

local HomeX;
local HomeY;

local Comb;

local Pollen;
local RandomPlant;

local WaterX;
local WaterY;

local BeeState;
// 0 - Fly Wander
// 1 - Go to ground
// 2 - Roam Ground
// 3 - Rest
// 4 - Attack
// 5 - Go Home (Gather, Gossip, Construct)  REQUIRES DAY-NIGHT CYCLE
// 6 - Collect Pollen 
// 7 - Deposit Pollen
// 8 - Escape to Water

//lorry bumb :)
local lorry_wipf;

//function for bumbboxes, to see if bumb is in a wild hive
public func IsWild(){
	if(Comb && GetID(Comb) != BUMB){
		if(GetID(Comb) == BMBX) return(0);
		else return(1);
	}else{
		return(1);
	}
}

public func CanRoamFine(){
	//if its top or bottom verticies are touching the landscape, repath if cant see target
	if(GetContact(this(), -1) & CNAT_Bottom) return(PathFree(GetX(), GetY(), gX, gY));
	if(GetContact(this(), -1) & CNAT_Top) return(PathFree(GetX(), GetY(), gX, gY));
	return(GetPathLength(GetX(),GetY(),gX,gY));
}
public func IsPossessible() { return(1); }

private func SpecialRepr()
{
  CreateObject(BBEG, 0, 10);
  return(1);
}

private func CountMe()
{
  var ReprodSize = ReproductionAreaSize();
  var ReprodSizeHalb = ReprodSize  / -2;
  return(ObjectCount(MONS, ReprodSizeHalb, ReprodSizeHalb, ReprodSize , ReprodSize , OCF_Alive) + ObjectCount(BBEG, ReprodSizeHalb, ReprodSizeHalb, ReprodSize , ReprodSize , 0));
}

func InColony(){ return(HomeX != 0); }

func DayCheck(){
	return(IsDay());
}

func Initialize() {
	MovingHome = false;
	Pollen = 0;
	HomeX = 0;
	BeeState = 0;
	MaxBeenergy = 2000;
	Beenergy = MaxBeenergy;
  SetAction("Fly");
  gX=0;
  gY=0;
  return(1);
}

public func Birth(){
	Initialize();
}

func Activity(){
	if(Beenergy < 0) Beenergy = 0;
	
	if(GetXDir() > 0 && GetDir() == DIR_Left)  return(TurnRight());
	if(GetXDir() < 0 && GetDir() == DIR_Right) return(TurnLeft());
	
	//if possesed, dont do anything related to normal behaviour
	 if(GetEffect("PossessionSpell", this())){
		 SetPlrViewRange(300, this());
		 if(!Random(15)) Sound("Bsss");
		 BeeState = 0;
		 if(GetAction() == "Rest" || GetAction() == "Idle") SetAction("Fly");
		 if(GetContact(this(), -1) & CNAT_Bottom)
			if(GetAction() == "Fly") SetAction("Walk");
		   ;
		 return(0);
	 }else{
		 SetPlrViewRange(0, this());
	 }
	 
	
	if(Pollen > 15) Pollen = 15;
	if(Pollen >= RandomX(1,15)) CreateParticle("PSpark", 0, 0, RandomX(-10,10), -3, 25, RGBa(255,255,0));
	
	if(InLiquid() && Pollen > 0){
		for(var i = 0; i < Pollen; i++){
			CreateParticle("PxSpark", 0, 0, RandomX(-10,10), RandomX(-10,10), 25, RGBa(255,255,0));
		}
		Pollen = 0;
	}
	
	if(!Random(15)) Sound("Bsss");
	
	//if enemy is too far or in building, forget
	  if(ObjectDistance(this(), GrudgeTarget) >= 200 || Contained(GrudgeTarget)){
		  GrudgeTarget = 0;
	  }
	
	//retreat to remembered water location
	if(BeeState == 8){
		BUMBAI_Escape();
	}
	
	//Before anything, search for water below, in case it gets on fire.
	if(!WaterX){
		if(GetMaterial(0,30) == Material("Water")){
			WaterX = GetX();
			WaterY = GetY() + 35;
			DebugLog("a Bumb memorized the location of water.");
		}
	}
	
	//is on fire? ditch everything, go to the remembered water location.
	if(OnFire() && WaterX){
		BeeState = 8;
	}
	
	//Random Wandering (Sky)
  if(BeeState == 0){
	return(BUMBAI_Wander());
  }
  
  //Going to ground
  if(BeeState == 1){
	return(BUMBAI_GoGround());
  }
  
  //Roaming Ground
  if(BeeState == 2){
	  return(BUMBAI_WanderGround());
  }
  
  //Sleeping
  if(BeeState == 3){
	  return(BUMBAI_Sleep());
  }
  
  //Attacking (Ruthless!)
  if(BeeState == 4){
	 return(BUMBAI_Revenge());
  }
  
  //Idling in colony at night, includes constructing and gossiping
  if(BeeState == 5){
	  return(BUMBAI_WanderColony());
  }
  
  //Collecting Pollen From Plants
  if(BeeState == 6){
	  return(BUMBAI_FindPollen());
  }
  
  //Bring Honey to Combs
   if(BeeState == 7){
	  return(BUMBAI_DepositPollen());
  }
  
  
}

/* BUMB AI FUNCTIONS AND HELPERS, CALLED BY TIMER. */

//Changes the bumbs path, this is some basic pathfinding used by multiple states.
func BumbRepath(){
	if(GetAction() == "Idle") SetAction("Fly");
	if(GetAction() == "Walk") SetAction("Fly");
	if(!Random(20)|| Distance(GetX(),GetY(),gX,gY) <= 30 || GBackSolid(gX-GetY(),gY-GetY()) || GBackLiquid(gX-GetY(),gY-GetY()) || !CanRoamFine()){
		gX = GetX() + RandomX(-500,500);
		gY = GetY() + RandomX(-500,500);
	}
	
	//Avoid liquids
	  if(GBackLiquid(0, 30)){
		gX = GetX() + RandomX(-1000,1000);
		gY = GetY()-30;
		}
	
	if(gY < 50) gY = GetY() + RandomX(100,2000);
	if(GetX() < 70) gX = GetX() + RandomX(1000,5000);
	if(GetX() > LandscapeWidth() - 70) gX = GetX() - RandomX(1000,5000);
	
	SetCommand(this(), "MoveTo", ,gX,gY);
	//DebugLog("%v repathed to %d, %d",this(),gX,gY);
	return(1);
}

//behaviour for unstucking, for simple velocity changes to digging out.
//recently fixed, bumbs no longer rocket into the sky randomly
func BumbUnstuck(){
	var trsh = RandomX(100,250);
	if(GetSpeed() < 5 && GetAction() == "Fly"){
	if(GetContact(this(), -1) & CNAT_Bottom) SetYDir(GetYDir()-trsh);
	else if(GetContact(this(),-1) & CNAT_Top) SetYDir(GetYDir()+trsh);
	if(GetContact(this(),-1) & COMD_Left) SetXDir(GetXDir()+trsh);
	else if(GetContact(this(),-1) & COMD_Right) SetXDir(GetXDir()-trsh);
	}
	
	if(Stuck() && GetAction() == "Fly"){
		SetAction("Attack");
		DigFree(GetX(),GetY(),12);
		if(!Stuck()) Sound("Dig");
	}
}

//State 0: Basic Random Movement.
func BUMBAI_Wander(){
	BumbRepath();
	BumbUnstuck();
	
	if(Beenergy <= 200) BeeState = 1;
	
	Beenergy -= RandomX(1,10);
	
	if(GrudgeTarget && ObjectDistance(this(),GrudgeTarget) < 100 && !Contained(GrudgeTarget)){
		BeeState = 4;
	    return(0);
	}
	
	//Colony Starting
	if(ColoniesEnabled()){
	if(GetMaterial(0,0) == Material("Tunnel") && !InColony() && !Random(80) && CheckCombSpace(false)){
		DebugLog("A Bumb Set up a Colony!");
		HomeX = GetX();
		HomeY = GetY();
		//Colony Setup
		SetAction("Attack");
		Sound("Dig*");
		Comb = CreateObject(HNCB);
		LocalN("DesignatedBumb",Comb) = this();
		LocalN("PollenAmount", Comb) = 0;
		JoinColony(800);
	}
	
	//inviting strays if part of colony
	if(InColony() && !Random(40)){
		JoinColony(100);
	}
	
	//is day over? go to colony and chill
	if(InColony() && !DayCheck()){
		BeeState = 5;
	}
	}
	
	//Collect Pollen
	if(Comb){
		if(!Random(8)){
			BeeState = 6;
			var PlantList;
			PlantList = FindObjects(Find_Distance(200),Find_Func("CanBePollinated"));
			RandomPlant = PlantList[RandomX(0,GetLength(PlantList))];
			if(!RandomPlant) BeeState = 0;
			return(0);
		}
		
		//or.. deposit it
		if(!Random(8) && Comb && Pollen > 0 && Comb->~CombAvailable()){
			BeeState = 7;
			return(0);
		}
	}
}

//State 1: Going to ground. (Transition to State 2)
func BUMBAI_GoGround(){
	//doesnt use normal repath function as this uses a variation of the reptather.
	gX = GetX() + RandomX(-10,10);
	  gY= LandscapeHeight();
	  
	  if(GetContact(this(), -1) & CNAT_Bottom){
		  if(GetEnergy() <= 25 || !DayCheck()){
			  BeeState = 3;
			  return(0);
		  }
		  else BeeState = 2;
	  }
	  
	  //Avoid liquids
	  if(GBackLiquid(0, 30)){
		gX = GetX() + RandomX(-1000,1000);
		gY = GetY()-30;
		}
	
	if(gY < 50) gY = GetY() + RandomX(100,2000);
	if(GetX() < 70) gX = GetX() + RandomX(1000,5000);
	if(GetX() > LandscapeWidth() - 70) gX = GetX() - RandomX(1000,5000);
	  
	 Beenergy -= RandomX(1,5);
	 SetCommand(this(), "MoveTo", ,gX,gY);
	  
	  if(GrudgeTarget && ObjectDistance(this(),GrudgeTarget) < 100 && !Contained(GrudgeTarget)){
		BeeState = 4;
		return(0);
	}
}

//State 2: Roaming Ground and Eating all the honey on the floor.
func BUMBAI_WanderGround(){
	if(GetContact(this(), -1) & CNAT_Bottom){
		if(GetAction() == "Idle") SetAction("Walk");
		if(GetAction() == "Fly") SetAction("Walk");
	  }else{
		  if(GetAction() == "Tumble") return(0);
		 SetAction("Idle");
		 return(0);
	  }
	  
	  //stop any command so you dont go left and right constantly
	  if(GetCommand()) FinishCommand();
	  
	  if(!Random(7)){
	    var direction = Random(3);
		if(!direction){
			Walkdir = COMD_Stop;
		}else if(direction == 1){
			Walkdir = COMD_Left;
		}else{
			Walkdir = COMD_Right;
		}
		
		SetComDir(Walkdir);
	  }
	  
	  if(GetContact(this(), -1) & CNAT_Left) SetComDir(COMD_Right);
	  else if(GetContact(this(), -1) & CNAT_Right) SetComDir(COMD_Left);
	  
	  	//Eat Honey
		if(!Random(9) && FindObject2(Find_ID(HONY), Find_Distance(20), Find_NoContainer())){
			var Food = FindObject2(Find_ID(HONY), Find_Distance(20), Find_NoContainer());
			DoCon(RandomX(-30,-5), Food);
			DoEnergy(5);
			Sound("Corrode");
		}
	  
	  Beenergy += RandomX(1,20);
	  if(Beenergy >= MaxBeenergy) BeeState = 0;
	  
	  if(GrudgeTarget && ObjectDistance(this(),GrudgeTarget) < 100 && !Contained(GrudgeTarget)){
		BeeState = 4;
		return(0);
	 }
	 
	 if(InLiquid()) BeeState = 0;
}

//State 3: eepy
func BUMBAI_Sleep(){
	if(GetAction() != "Rest") SetAction("Rest");
	  if(GetAction() == "Rest"){
		  if(!Random(3))
		  DoEnergy(1);
			 
	    if(!Random(6))
	    CreateParticle("Zzz",0,0, RandomX(-5,5), -3, RandomX(25,50), RGBa(255,255,255), ,true);
		Beenergy += RandomX(1,25);
	  }
	  
	  if(Beenergy >= MaxBeenergy){
		  BeeState = 0;
		  SetAction("Fly");
	  }
	  
	  if(InLiquid()) BeeState = 0;
}

//State 4: Chase off and attack predators or unfortunate clonks that steal its honey.
func BUMBAI_Revenge(){
	if(GrudgeTarget == 0) BeeState = 0;
	  if(BeeState == 0) return(0);
	  
	  if(GetAction() == "Idle") SetAction("Fly");
	  if(GetAction() == "Walk") SetAction("Fly");
	  
	  if(!GetAlive(GrudgeTarget)){
		BeeState = 0;
		return(0);
	  }
	  
	  if(ObjectDistance(this(), GrudgeTarget) <= 15 && (GetAction() == "Fly" || GetAction() == "FlyTurn") && !Contained(GrudgeTarget)){
		  SetAction("Attack");
		  Sound("Sting");
		  Punch(GrudgeTarget, RandomX(1,5));
		  InformBumbs();
	  }
	  
	  //fight cancellation logic moved to another part of the script.
	  
	  Beenergy -= RandomX(1,10);
	  
	  BumbUnstuck();
	  SetCommand(this(), "MoveTo", GrudgeTarget);
}

//State 5: Random wandering, but if a bumb has a colony and its night time.
func BUMBAI_WanderColony(){
	BumbUnstuck();
	if(Distance(GetX(),GetY(),HomeX, HomeY) > 650 || MovingHome){
		  SetCommand(this(), "MoveTo", ,HomeX,HomeY);
		  MovingHome = true;
	}else{
		BumbRepath();
		//Make Comb if you dont have one
		if(!Comb && CheckCombSpace(true) && GetMaterial(0,0) == Material("Tunnel")){
		  DebugLog("Bumb created a comb!");
		  SetAction("Attack");
		  Sound("Dig*");
		  Comb = CreateObject(HNCB);
		  LocalN("DesignatedBumb",Comb) = this();
		  LocalN("PollenAmount", Comb) = 0;
		}
		
		//gossip
		if(!Random(30)) InformBumbs();
		
		if(Beenergy <= 200) BeeState = 1;
	
		Beenergy -= RandomX(1,10);
		
		SetCommand(this(), "MoveTo", ,gX,gY);
	}
	
	if(Distance(GetX(),GetY(),HomeX, HomeY) < 100)
		  MovingHome = false;
	  
	  if(DayCheck()) BeeState = 0;
	  
	  if(GrudgeTarget && ObjectDistance(this(),GrudgeTarget) < 100 && !Contained(GrudgeTarget)){
		BeeState = 4;
		return(0);
		}
}

//State 6: Find and Collect pollen from flowers grass or trees. (Later, when proper flowers are added, only search flowers.)
func BUMBAI_FindPollen(){
	if(!Random(15)) Sound("Bsss");
	  if(GetAction() == "Idle") SetAction("Fly");
	  if(GetAction() == "Walk") SetAction("Fly");
	  
	  BumbUnstuck();
	  SetCommand(this(), "MoveTo",RandomPlant);
	  
	  if(!RandomPlant){
		  BeeState = 0;
		  return(0);
	  }
	  
	  if(ObjectDistance(this(), RandomPlant) < 23){
		  Pollen += RandomX(1,2);
		  for(var i = 0; i < RandomX(1,3); i++){
			CreateParticle("PxSpark", 0, 0, RandomX(-20,20), RandomX(-5,20), 25, RGBa(255,255,0));
		  }
		  SetAction("Attack");
		  Sound("Sting");
		  RandomPlant->~OnPollination();
		  if(Pollen > 13 && Comb)
			  BeeState = 7;
		  else
		  BeeState = 0;
	  }
	  
	  if(!Random(50)) BeeState = 0;
	  
	  if(Beenergy <= 200) BeeState = 1;
	
	  Beenergy -= RandomX(1,8);
	  
	  if(GrudgeTarget && ObjectDistance(this(),GrudgeTarget) < 100 && !Contained(GrudgeTarget)){
		BeeState = 4;
		return(0);
	 }
}

//State 7: Retrieve collected pollen to a comb or bumbbox.
func BUMBAI_DepositPollen(){
		   if(!Random(15)) Sound("Bsss");
	  if(GetAction() == "Idle") SetAction("Fly");
	  if(GetAction() == "Walk") SetAction("Fly");
	  
	  if(!Comb){
		  BeeState = 0;
		  return(0);
	  }
	  
	  BumbUnstuck();
	  SetCommand(this(), "MoveTo",Comb);
	  
	  if(ObjectDistance(this(), Comb) < 10){
		  LocalN("PollenAmount", Comb) += Pollen;
		  Pollen = 0;
		  SetAction("Attack");
		  Sound("Sting");
		  BeeState = 0;
	  }
	  
	  if(Beenergy <= 200) BeeState = 1;
	
	  Beenergy -= RandomX(1,8);
	  
	  if(GrudgeTarget && ObjectDistance(this(),GrudgeTarget) < 100 && !Contained(GrudgeTarget)){
		BeeState = 4;
		return(0);
	  }
}

//State 8: Escape to nearby water if you memorized a location, rarely effective but who cares
func BUMBAI_Escape(){
	if(GetAction() == "Idle") SetAction("Fly");
		if(GetAction() == "Walk") SetAction("Fly");
		
		SetCommand(this(), "MoveTo", ,WaterX,WaterY);
		
		if(!OnFire()) BeeState = 0;
		
		Beenergy -= RandomX(2,8);
}

/* end of ai */

protected func CatchBlow(int iLevel, object pByObject)
{
  if (!Random(3)) Sound("BumbHurt*");
  
  GrudgeTarget = 0;
  if(GetAlive(pByObject)){
	  GrudgeTarget = pByObject;
  }else{
	  GrudgeTarget = GetCursor(GetController(pByObject)); 
  }

  if(GrudgeTarget != 0) InformBumbs();
  
  //Wake up if not tired
  if(Beenergy >= (MaxBeenergy/3)*2) BeeState = 4;
  
  return(1);
}

protected func Death() 
{
	if(lorry_wipf) RemoveObject(lorry_wipf);
  Sound("BumbDie*");
  SetDir(0);
  ChangeDef(DBUB);
  SetAction("Dead");
  return(1);
}

protected func RejectCollect(c4ID, pObject)
{
 var iEffectNumber, pSorcerer;
 if (iEffectNumber = GetEffect("PossessionSpell", this()))
  if (pSorcerer = EffectVar(0, this(), iEffectNumber))
   if (!GetEffect("IntCollectionDelay", this()))
    if (!pSorcerer->Contents())
     if (!(pObject-GetOCF() & OCF_Living))
      Collect(pObject, pSorcerer);
 return(1);
}

func RejectEntrance(pIntoObject,a,b,c){
	if(GetID(pIntoObject) == LORY && FindObject2(Find_ID(BUMB),Find_Container(pIntoObject))) return(1);
	if(Contained()) return(_inherited(pIntoObject,a,b,c));
	if(GetID(pIntoObject) == LORY && GetAction() != "Tumble") return(1);
	else if(GetID(pIntoObject) == LORY && GetAction() == "Tumble") return(0);
	if(GetAction() == "Rest" || WildcardMatch(GetAction(), "*Walk*")) return(_inherited(pIntoObject,a,b,c));
	return(1);
}

//lorry bumb :)
public func Entrance(obj) 
{
	
  // Ist in eine Lore gegangen?
  if (obj->~IsLorry())
  {
    // Lorenwipf suchen, der zu der Lore gehört
    lorry_wipf = FindObject(LBUB, 0,0,0,0, 0,0, obj);

    // Lorenwipf gefunden?
    if (!lorry_wipf) 
    {
      // Erzeugen wir halt einen
      lorry_wipf = CreateObject(LBUB, 0,0, -1);

      // An die Lore kleben
      lorry_wipf->AttachTo(obj);
	  
	  lorry_wipf->SetDir(GetDir());
	  
	  if(OnFire()) Incinerate(lorry_wipf);
    }
  }
}

func Departure(pObj){
	if(lorry_wipf) RemoveObject(lorry_wipf);
	SetAction("Tumble");
}

public func TurnRight()
{
  if (GetXDir() < 0) SetXDir(0);
  SetDir(DIR_Right, this());
  SetComDir(COMD_Right); 
  return(1);
}

public func TurnLeft()
{
  if (GetXDir() > 0) SetXDir(0);
  SetDir(DIR_Left);
  SetComDir(COMD_Left); 
  return(1);
}

public func InformBumbs(){
	var LocalBumbs = FindObjects(Find_ID(BUMB), Find_Distance(500));
	for(var bumb in LocalBumbs){
		if(LocalN("GrudgeTarget", bumb) != GrudgeTarget){
		LocalN("GrudgeTarget", bumb) = GrudgeTarget;
		}else if(LocalN("WaterX", bumb) != 0){
		LocalN("WaterX", bumb) = WaterX;
		LocalN("WaterY", bumb) = WaterY;
		}
	}
}

public func JoinColony(int Distance){
	var LocalBumbs = FindObjects(Find_ID(BUMB), Find_Distance(Distance));
	for(var bumb in LocalBumbs){
		if(!ObjectCall(bumb, "InColony")){
			DebugLog("Bumb Recruited");
			LocalN("HomeX", bumb) = HomeX;
			LocalN("HomeY", bumb) = HomeY;
		}
	}
}

public func CheckCombSpace(bool ColonyMode){
	//in caves only.
	if(GetMaterial(0,0) != Material("Tunnel")) return(0);
	//no combs on the edges of walls :P
	if(GBackSky(-20,0) || GBackSky(20,0) || GBackSky(0,20) || GBackSky(0,-20)) return(0);
	
	//no combs in the ground
	if(GBackSolid(0,0)) return(0);
	
	//dont place near any Player-Made structures
	if(FindObject2(Find_Category(C4D_Structure), Find_Distance(450))) return(0);
	
	//Dont get too close to other honeycombs
	if(FindObject2(Find_ID(HNCB), Find_Distance(25))) return(0);
	
	//Is the Bee a part of a colony? Check if any other combs are nearby first.
	if(ColonyMode){
		if(!FindObject2(Find_ID(HNCB), Find_Distance(35))) return(0);
	}
	
	//all fine? go ahead, place it :D
	return(1);
}

//Possesion Control
protected func ControlCommand(szCommand, pTarget, iTx, iTy)
{
 if (szCommand == "MoveTo")
  return(SetCommand(this(),szCommand, pTarget, iTx, iTy));
 return(0);
}

protected func ContainedLeft(object caller)
{
  [$TxtMovement$]
  SetCommand(this(), "None");
  if(!GetPlrJumpAndRunControl(caller->GetController()))
  {
      TurnLeft();
  }
  return(1);
}

protected func ContainedRight(object caller)
{
  [$TxtMovement$]
  SetCommand(this(), "None");
  if(!GetPlrJumpAndRunControl(caller->GetController()))
  {
     TurnRight();
  }

  return(1);
}

protected func ContainedUp(object caller)
{
 [$TxtMovement$]
 SetCommand(this(), "None");

 if(!GetPlrJumpAndRunControl(caller->GetController())){
   SetComDir(COMD_Up);
   if(GetContact(this(), -1) & CNAT_Bottom)
			if(GetAction() == "Walk") SetAction("Fly");
		   ;;
 }
 return(1);
}

protected func ContainedDown(object caller)
{
 [$TxtMovement$]
 SetCommand(this(), "None");
 if(Contained()) SetCommand(this, "Exit");
 if(!GetPlrJumpAndRunControl(caller->GetController())){
   SetComDir(COMD_Down);
 }
  return(1);
 }
 
protected func ContainedDigDouble()
{
 [$TxtLeave$]
 RemoveEffect("PossessionSpell", this());
 return(1);
}

protected func ContainedDig()
{
 [$TxtLead$|Image=ZAPN]
 if(InColony()){
	 SetCommand(this(), "MoveTo", ,HomeX,HomeY);
 }
}

protected func ContainedThrow(object caller)
{
 [$TxtSting$]
 if(!GetPlrJumpAndRunControl(caller->GetController()) && GetAction() == "Fly" && FindObject2(Find_OCF(OCF_Alive), Find_Distance(10), Find_OCF(OCF_NotContained), Find_Exclude(this()))){
   SetAction("Attack");
   Sound("Sting");
   var power = RandomX(1,5);
   Punch(FindObject2(Find_OCF(OCF_Alive), Find_Distance(18), Find_OCF(OCF_NotContained), Find_Exclude(this())), power);
   FindObject2(Find_OCF(OCF_Alive), Find_Distance(18), Find_OCF(OCF_NotContained), Find_Exclude(this()))->CatchBlow(power,this());
 }
 return(1);
}

private func ClearDir(bool fX)
{
  if(fX && GetXDir())
  {
    if(GetXDir() > 0) SetXDir(Max(GetXDir() - 2, 0));
    else SetXDir(Min(GetXDir() + 2, 0));
  }
  if(!fX && GetYDir())
  {
    if(GetYDir() > 0) SetYDir(Max(GetYDir() - 2, 0));
    else SetYDir(Min(GetYDir() + 2, 0));
  }
}

public func ContainedUpdate(object self, int comdir, bool dig, bool throw)
{
    SetComDir(comdir);
	
	if(throw && GetAction() == "Fly" && FindObject2(Find_OCF(OCF_Alive), Find_Distance(18), Find_OCF(OCF_NotContained), Find_Exclude(this()))){
		var target = FindObject2(Find_OCF(OCF_Alive), Find_Distance(18), Find_OCF(OCF_NotContained), Find_Exclude(this()));
		SetAction("Attack");
		Sound("Sting");
		var power = RandomX(1,5);
		Punch(target, power);
		if(GetAlive(target))
		target->CatchBlow(power,this());
	}
		   
    if(comdir == COMD_Up || comdir == COMD_UpLeft || comdir == COMD_UpRight)
		if(GetContact(this(), -1) & CNAT_Bottom)
			if(GetAction() == "Walk") SetAction("Fly");
		   ;;
    
  return(1);
}

//essences
public func HasEssence(){
	return(1);
}

public func EssenceInfo(type, object pObj){
	//Color - Color
	//PHYSICAL - physical :)
	//Start - OnStart, only plays once even when multiple are combined
	//End - OnEnd, only plays once even when multiple are combined
	if(type == "Color") return(RGBa(241,162,0));
	if(type == "Jump") return(2800);
}

func Sting(){
SetXDir(0); SetYDir(0);
}
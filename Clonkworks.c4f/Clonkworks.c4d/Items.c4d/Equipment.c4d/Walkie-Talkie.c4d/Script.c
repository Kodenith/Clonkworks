/*-- Neues Script --*/

#strict 2

local Msg;
local Target;
local Activator;

local ArchivedTalkList;
local MenuOn;

func Initialize() {
  SetAction("Off");
  return(1);
}

public func Activate(pByObj)
{
  [$Message$]
  MenuOn = true;
  SetAction("On");
  SetPicture(70, 0, 64, 64);
  Sound("Click");
  Activator = pByObj;
  CreateMenu(WKTK, pByObj, this(),0,"$NoSignal$");
  var iTalkies = FindObjects(Find_ID(WKTK), Find_Exclude(this()));
  ArchivedTalkList = iTalkies;
  for(var talkie in iTalkies){
	  if(GetCon(talkie) < 100) continue;
	  AddMenuItem("$Message$: %s", "StartTalking", GetID(talkie), Activator, 0, talkie, ,6,GetColorDw(talkie));
  }
  OnMenuSelection(0, Activator);
  return(1);
}

protected func StartTalking(Foo, Talkie){
	if(GetOCF(Activator) & OCF_CrewMember) SetPlrView(GetOwner(Activator), GetCursor(0,GetOwner(Activator)));
	MenuOn = false;
	Target = Talkie;
	if(!CheckTargetExists()) return(0);
	Sound("Click");
	CallMessageBoard(this(), false, Format("$Message$: %s", GetName(Target)), GetController(Activator));
	return(1);
}

protected func CheckTargetExists(){
	if(Target){
		return(true);
	}else{
		SetPicture(6, 0, 64, 64);
		MenuOn = false;
		SetAction("Off");
		Message("$SendFail$", this());
		Sound("Error");
		return(false);
	}
}

func InputCallback(szInput){
	SetAction("Off");  SetPicture(6, 0, 64, 64);
	if(!szInput || !CheckTargetExists()) return(0);
	Sound("Click");
	Target->Read(szInput);
}

public func Read(szMsg){
	Msg = szMsg;
	CustomMessage(Msg, this(), GetOwner(this()), 0, 0, RGB(0,255,255),,,,2);
	SetAction("Read");
	SetPicture(70, 0, 64, 64);
	Sound("Ding");
}

func OnMenuSelection(int iItemIndex, object pMenuObject){
	if(ContentsCount(WKTK, pMenuObject) && (GetOCF(pMenuObject) & OCF_CrewMember)){
		if(ArchivedTalkList[iItemIndex]){
			SetPlrView(GetOwner(pMenuObject), ArchivedTalkList[iItemIndex]);
		}
	}
}

protected func Update(){
	if(GetAction() == "Idle") SetAction("Off");
	
	if(GetAction() == "Read" && GetActTime(this()) > GetLength(Msg) * 3){
		SetPicture(6, 0, 64, 64);
		SetAction("Off");
		return(0);
	}
	
	if(!Contained()){
		SetPicture(6, 0, 64, 64);
		SetAction("Off");
		MenuOn = false;
	}
	
	if(MenuOn && Activator)
	if(GetAction() == "On" && GetMenu(Activator) != WKTK){
		SetPicture(6, 0, 64, 64);
		SetAction("Off");
		MenuOn = false;
		};
}

public func IsAnvilProduct(){ return(true); }
public func IsAdvancedProduct(){ return(true); }

protected func Hit()
{
  Sound("MetalHit*");
}

protected func Hit3(){
	  CastParticles("MSpark",5,25,0,0,15,30,RGB(255,223,127),RGB(255,223,127));
	  Sound("AnvilWork*");
	  RemoveObject(this());
}
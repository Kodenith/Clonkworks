/*-- Neues Script --*/
#strict 2
#include CXEC
#include BAS1
local dirRight;

func Initialize(){
  dirRight = 0;
  Sound("Click");
  Message("$TxtLeft$",this);
}

func RejectCollect(pId,pObj){
	if(Contained(pObj)) return(1);
  if(Inside(GetX(pObj),GetX()-12,(GetX()-12)+25) && Inside(GetY(pObj),GetY()-10,(GetY()-10)+14)) Enter(this,pObj);
}

func Collection(){ Sound("Clonk"); }
func ControlLeft(pClonk){
  [$TxtCtr$]
  if(!dirRight) return(1);
  dirRight = 0;
  Sound("Click");
  Message("$TxtLeft$",this);
}
func ControlRight(pClonk){
  [$TxtCtr$]
  if(dirRight) return(1);
  dirRight = 1;
  Sound("Click");
  Message("$TxtRight$",this);
}

private func BanList(){
  return([FTG1,FTG2,FTG3,FTG4,FTG5,FTG6,FTG7,INSR,CNVS,CVS2,CHM2,ANV2,EXDR,EXDC,CT5P,TWR1,TWR2,FND2]);
}

func Decide(){
  if(GetCon() < 100 || !EnergyCheck(3500) || !ActIdle() || !Contents(0)){
    return(0);
  }

  var Finders;
  Finders = FindObjects(Find_OnLine(-100,0,100,0),Find_Or(Find_Category(C4D_Vehicle),Find_Category(C4D_Structure)),Find_Or(Find_OCF(OCF_Container),Find_ID(QUAR)),Find_NoContainer(),Find_Exclude(this()),Find_Not(Find_ID(INSR)));
  if(!Finders) return(0);

  for(var i in Finders){
    if(dirRight && GetX(i) < GetX()) continue;
    if(!dirRight && GetX(i) > GetX()) continue;
    if(!PathFree(GetX(),GetY(),GetX(i),GetY(i))) continue;
    if(InArray(i,BanList()) == -1){
      var Proj = CreateObject(INS2);
      Proj->Initialize();
      Enter(Proj,Contents(0));
      if(dirRight){
        SetAction("ShootRight");
        SetX(GetX()+16,Proj);
        SetXDir(25,Proj);
      }else{
        SetAction("ShootLeft");
        SetX(GetX()-16,Proj);
        SetXDir(-25,Proj);
      }
      Sound("Chuff");
      break;
    }
  }
}

func Damage(){
  if(GetDamage() > 50){
  while(Contents(0)) Exit(Contents(0));
  if(FindObject(CNMT))
    Split2Components();
  else RemoveObject();
    CreateObject(ROCK)->Explode(25);
  }
}
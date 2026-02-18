/*-- Neues Objekt --*/

#strict 2

local CrTime;

func Initialize(){
  CrTime = FrameCounter();
}

private func BanList(){
  return([FTG1,FTG2,FTG3,FTG4,FTG5,FTG6,FTG7,INSR,INS2,CNVS,CVS2,CHM2,ANV2,EXDR,EXDC,CT5P,TWR1,TWR2,FND2]);
}

func TryInsert(){

  SetYDir(0);
  if(FrameCounter() > CrTime+(38) || !Contents(0)){
    Exit(Contents(0));
    RemoveObject();
    return(0);
  }
  
  //set object graphics
  SetGraphics(0, this(), GetID(Contents(0)),1,1);
  //make sure size follows the same value as the objects size, and flip the object accordingly to direction
  if(GetXDir() > 0) SetObjDrawTransform(-(GetCon(Contents(0))*10),0,0,0,GetCon(Contents(0))*10,0,this,1);
  else SetObjDrawTransform((GetCon(Contents(0))*10),0,0,0,GetCon(Contents(0))*10,0,this,1);

  if(!Random(2)) CreateParticle("PxSpark",0,0,RandomX(0,-GetXDir()),RandomX(-2,2),RandomX(10,30),RGBa(255,255,255));
  for(var i in FindObjects(Find_AtPoint(),Find_NoContainer(),Find_Or(Find_Category(C4D_Vehicle),Find_Category(C4D_Structure)),Find_OCF(OCF_Container))){
      var list = BanList();
      if(InArray(GetID(i),list) != -1) continue;
      Contents(0)->Sound("Electric",0,this(),50,0,0,0,100);
      Enter(i,Contents(0));
      RemoveObject();
  }
}

func Destruction(){
  CastParticles("PxSpark",5,20,0,0,10,30,RGBa(255,255,255),RGBa(255,255,255));
}
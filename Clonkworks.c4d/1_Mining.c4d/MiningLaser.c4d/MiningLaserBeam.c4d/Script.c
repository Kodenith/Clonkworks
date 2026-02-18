#strict 2

local w, l, r, c, iTime, pDmgCallback, pAttach, pLight, iMaxDist, dx, dy, crossed;
local reflections;
local xLaser, yLaser;
local xs, ys; //Koordinaten des Schnittpunktes
local oldxs, oldys;
local aData;
local iCounter;
local pObstacle, wallhit;
local mx, my, ml;
//local oldml;
local active, Overcharge;
local iElement, iElementar;
local iCrossDist;
local newlaser;
local wallhitX, wallhitY;
local MiningCharge;

local laserendtimer; //erstellt die Visualisierung am Laserstrahlende erst mit Verzögerung

protected func Initialize()
{
  //SetObjectBlitMode(3);
  SetClrModulation(RGB(255,0,0));
  //oldml= 600;
  aData= [];
}

/* Interface (public functions) */

public func SetAngle(int iAngle) {
   //if(iAngle == 0) r= -180;
   //else r = iAngle-180;
   r = iAngle-180;
}
public func SetWidth(int iWidth) {
   w = 1000*iWidth/GetActMapVal("Facet","Laser",0,2);
}
public func SetMaxDistance(int iDist) {
   iMaxDist = iDist;
}
public func SetContinuous(int iDelay) {
  if(!iDelay) iDelay = 1;
  AddEffect("Continuous", this(), 1,iDelay, this());
}

public func Set(int iAngle, int iWidth, int iDist, int iT, object pDmg, object pAtt, int Element, int Elementar)
{
  // optional parameters
  if(!iWidth) iWidth = 3;
  if(!iDist) iDist = 300;
  // iT = 0: no out-fading
  iTime = iT;
	//reflections = iReflect; // how often the Ray should be reflected
  iElement= Element;
  iElementar= Elementar;
  // reset fade-timer
  c=0;
  // new attach and damage callback objects (optional)
  pDmgCallback = pDmg;
  pAttach = pAtt;
  if(pAttach) {
    dx = GetX()-GetX(pAttach);
    dy = GetY()-GetY(pAttach);
  }
  else 
  {
    dx = 0;
    dy = 0;
    xLaser= GetX();
    yLaser= GetY();
    pAttach= this;
  }

  // create light
  if(!pLight)
  pLight = CreateObject(LALI,0,0,-1);
  SetMaxDistance(iDist);
  SetAngle(iAngle);
  SetWidth(iWidth);
  if(crossed) crossed-> HitCheck();
  HitCheck();
  if(crossed) crossed-> HitCheck();
  DrawTransform();
  if(!GetEffect("BeamCheck"))
  AddEffect("BeamCheck", this(), 1,5, this());
 	// we are stuck?
	//if(Stuck())
		//RemoveObject();
}

public func GetAngle()  { return(r+180); }
public func GetWidth()  { return(w*GetActMapVal("Facet","Laser",0,2)/1000);  }
public func GetLength() { 
  if(GetAction()=="Idle")
    return(l*GetActMapVal("Facet","Laser",0,3)/1000);
  else
    return(l*GetActMapVal("Facet",GetAction(),0,3)/1000);
}

public func GetMaxDistance() { return(iMaxDist); }

public func LaserEnd(&x, &y) {
  x = +Sin(GetAngle(),GetLength()+1);
  y = -Cos(GetAngle(),GetLength()+1);
}

/* intern */

private func HitCheck() {
  pObstacle= 0;
  wallhit= 0;
  aData= [];
  iCrossDist=0;
  // calculate length
  mx=-Sin(r,iMaxDist);
  my=+Cos(r,iMaxDist);
  ml=0;
  if(pAttach)
  var xattach= GetX(pAttach)-GetX(), yattach= GetY(pAttach)-GetY();
  if(crossed) iCrossDist= Distance(GetX(),GetY(),oldxs,oldys);
  else iCrossDist= iMaxDist;
  while(++ml) 
  {
    if(GBackSolid(mx*ml/iMaxDist,my*ml/iMaxDist)) {wallhit= true; break;}
    if(ml<=iCrossDist)
    var pObs= FindObject2(Find_Not(Find_Distance(20,xattach,yattach)),Find_Distance(iCrossDist,0,0),Find_Distance(15,mx*ml/iMaxDist,my*ml/iMaxDist), Find_Exclude(pAttach), Find_Exclude(this), Find_Or(Find_Func("IsBulletTarget"),Find_OCF(OCF_Alive)));
    if(pObs) {pObstacle= pObs; break;}
    if(ml>=iMaxDist) break;
    //if(ml>=iCrossDist) {ml= iMaxDist; break;}
  }
   //if(!pObstacle && !wallhit) ml= iMaxDist;
   iCounter= 0;  
   aData[iCounter] = [];
   aData[iCounter][0] = GetX()+mx*ml/iMaxDist;
   aData[iCounter][1] = GetY()+my*ml/iMaxDist;
   iCounter++;
   oldxs= 0; oldys= 0;
  
  //ml= 0;
 
  //if(GetClrModulation(this) == RGB(255,0,0))
  for(var pLaser in FindObjects(Find_Exclude(this),Find_ID(LAS2)))
  {
     var x1= GetX(pLaser);//GetX(LocalN("pAttach",pLaser));
     var y1= GetY(pLaser);//GetY(LocalN("pAttach",pLaser));
     var x2, y2;
     if(pLaser-> HasObstacle() || LocalN("crossed",pLaser) && LocalN("crossed",pLaser)!= this)
     {
         x2= x1+LocalN("mx",pLaser)*LocalN("ml",pLaser)/LocalN("iMaxDist",pLaser);
         y2= y1+LocalN("my",pLaser)*LocalN("ml",pLaser)/LocalN("iMaxDist",pLaser);
     }
     else
     {
         //x2= x1+LocalN("mx",pLaser)*LocalN("ml",pLaser)/LocalN("iMaxDist",pLaser);
         //y2= y1+LocalN("my",pLaser)*LocalN("ml",pLaser)/LocalN("iMaxDist",pLaser);
         x2= x1-Sin(LocalN("r",pLaser),LocalN("iMaxDist",pLaser));
         y2= y1+Cos(LocalN("r",pLaser),LocalN("iMaxDist",pLaser));
     }
     var x3= GetX();//GetX(pAttach)+AbsX(dx);
     var y3= GetY();//GetY(pAttach)+AbsY(dy);
     var x4= /*x3+mx;*/x3+mx*ml/iMaxDist;//
     var y4= /*y3+my;*/y3+my*ml/iMaxDist;//
     
     //var vxL= x4-x3;
     //var vyL= y4-y3;
     //var vx= x2-x1;
     //var vy= y2-y1;
     
     //var xs, ys; //Koordinaten des Schnittpunktes
       //CreateParticle("PSpark",AbsX(x1),AbsY(y1),0,0,100,RGBa(5,130,108,0));
       //CreateParticle("PSpark",AbsX(x2),AbsY(y2),0,0,100,RGBa(5,130,108,0));
       //CreateParticle("PSpark",AbsX(x3),AbsY(y3),0,0,100,RGBa(5,130,8,0));
       //CreateParticle("PSpark",AbsX(x4),AbsY(y4),0,0,100,RGBa(5,130,8,0));
     xs=Abs(((x4-x3)*((x2*y1)-(x1*y2))-((x2-x1)*((x4*y3)-(x3*y4))))/(((y4-y3)*(x2-x1))-((y2-y1)*(x4-x3))));
     ys=Abs(((y1-y2)*((x4*y3)-(x3*y4))-((y3-y4)*((x2*y1)-(x1*y2))))/(((y4-y3)*(x2-x1))-((y2-y1)*(x4-x3))));
     
     
     //Message(Format("x1/y1:%d,%d | x2/y2:%d,%d |x3/y3:%d,%d|x4/y4:%d,%d",x1,y1,x2,y2,x3,y3,x4,y4/*,xs,ys*/));
     //Message(Format("xs/ys:%d,%d",xs,ys),pAttach);
     if(x1<x2){if(Inside(xs,x1,x2)){} else {xs= 0; ys= 0;}}
     else if(x1>x2){if(Inside(xs,x2,x1)){} else {xs= 0; ys= 0;}}
     //else {xs= 0; ys= 0;}
     if(x3<x4){if(Inside(xs,x3,x4)){} else {xs= 0; ys= 0;}}
     else if(x3>x4){if(Inside(xs,x4,x3)){} else {xs= 0; ys= 0;}}
     //else {xs= 0; ys= 0;}
     
     if(y1<y2){if(Inside(ys,y1,y2)){} else {xs= 0; ys= 0;}}
     else if(y1>y2){if(Inside(ys,y2,y1)){} else {xs= 0; ys= 0;}}
     //else {xs= 0; ys= 0;}
     if(y3<y4){if(Inside(ys,y3,y4)){} else {xs= 0; ys= 0;}}
     else if(y3>y4){if(Inside(ys,y4,y3)){} else {xs= 0; ys= 0;}}
     //else {xs= 0; ys= 0;}
     
     //if(xs && ys) if(FindObject2(Find_ID(_BCR),Find_OnLine(AbsX(x3), AbsY(y3), xs, ys))) {Message("Found"); xs=0; ys= 0;}
     //if(LocalN("xs",pLaser) && LocalN("ys",pLaser)) if(FindObject2(Find_ID(_BCR),Find_OnLine(AbsX(x1), AbsY(y1), xs, ys))) {/*Message("That",this);*/ xs=0; ys= 0;}
     //if(Distance(GetX(),GetY(),xs,ys) < 10) {xs= 0; ys= 0;}
     if(xs && ys)
     {
      
      aData[iCounter] = [];
      aData[iCounter][0] = xs;
      aData[iCounter][1] = ys;
      //CreateParticle("PSpark",AbsX(xs),AbsY(ys),0,0,100,RGBa(0,58,108,0));
      iCounter++;
      /*if(Distance(x3,y3,xs,ys) <= Distance(x3,y3,oldxs,oldys)|| !oldxs && !oldys)
      {
        oldxs= xs;
        oldys= ys;
      }
      else continue;*/
     //crossed= true;
     //CreateObject(_BCR,AbsX(xs),AbsY(ys));
     }   
  }
  var iMinDist = -1; var iIndex = -1;
  for(var i=0; i<iCounter; i++)
  {
    var iDist = Distance(GetX(this),GetY(this),aData[i][0],aData[i][1]);
    if(iMinDist == -1 || iMinDist > iDist)
    {
    iMinDist = iDist;
    iIndex = i;
    }
  }
  iCounter= 0;
  //if(GetClrModulation(this) == RGB(255,0,0))
  //Message(Format("%v %v| %v",iMinDist, iIndex, aData));
  //Message(Format("%v",aData));
  //Message(Format("%d",GetLength(aData)));
  //if(iIndex== 0) Overcharge= 0;
  if(iIndex != -1)
  {
    oldxs= aData[iIndex][0];
    oldys= aData[iIndex][1];
  }
  else{ crossed= false; /*iMaxDist= 600;oldxs= 0; oldys= 0;*/}
  iIndex= 0;
  crossed= false;
  for(var pLaser in FindObjects(Find_Exclude(this),Find_ID(LAS2)))
    {
        if(Inside(LocalN("oldxs",pLaser), oldxs-10, oldxs+10))
         if(Inside(LocalN("oldys",pLaser), oldys-10, oldys+10))
          {
             mx= oldxs;
             my= oldys;
             ml= Distance(GetX(),GetY(),mx,my);
             //oldml= ml;
             //iMaxDist= ml+10;
             l = 1000*ml/GetActMapVal("Facet",GetAction(),0,3);
             crossed= pLaser;
             break; 
          }
    }
  //Message(Format("%d %d",oldxs,oldys));
  if(crossed) CreateParticle("PSpark",AbsX(mx),AbsY(my),0,0,100,(GetClrModulation()+crossed->GetClrModulation())/2);
  if(crossed&& iElement == LocalN("iElement",crossed)/*&& iElementar == -1 && LocalN("iElementar",crossed)== -1*/)
  {
    //CreateParticle("Terashock",AbsX(mx),AbsY(my),0,0,Overcharge*12,RGBa(255,58,58,0)); 
    Overcharge++; 
    //Message(Format("%d",GetAngle()),this);
    if(Overcharge>2)
    { 
      if(!newlaser && !LocalN("newlaser",crossed)) 
      {
       /*var pMedal = FindObject(IF_M);
		   if(pMedal)
		   {
		     Reward(pMedal,GetOwner(crossed));
			   Reward(pMedal,GetOwner(this));
			 }*/
       newlaser = CreateObject(LAS4, AbsX(mx),AbsY(my), GetOwner());
       /*
       var newangle;
       if(Inside(r,-90,90) && Inside(LocalN("r",crossed),-90,90))
        newangle= 180+((r+LocalN("r",crossed))/2);
       else 
        //newangle= r+LocalN("r",crossed); Falls /2 nicht passt, dann den hier wieder nehmen
        newangle= (r+LocalN("r",crossed))/2;
       */
       var newangle;
       if(Inside(GetAngle(),-90,90) && Inside(crossed->GetAngle(),-90,90))
       {
        if(GetAngle() > 0 && crossed->GetAngle() > 0 || GetAngle() < 0 && crossed->GetAngle() < 0)
          newangle= (GetAngle()+crossed->GetAngle())/2; //Dieser Winkel wäre der zu erwartende, aber da kann man nicht soo viel am Boden treffen 
        else newangle= (GetAngle()+crossed->GetAngle())/2;
       }
       else if((!Inside(GetAngle(),-90,90) && Inside(crossed->GetAngle(),-90,90))||(Inside(GetAngle(),-90,90) && !Inside(crossed->GetAngle(),-90,90)))
       {
        //if(GetAngle() > 0 && crossed->GetAngle() > 0 || GetAngle() < 0 && crossed->GetAngle() < 0)
        //  newangle= (GetAngle()+crossed->GetAngle())/2; //Dieser Winkel wäre der zu erwartende, aber da kann man nicht soo viel am Boden treffen 
        if((Abs(GetAngle()) + Abs(crossed->GetAngle())) > 180) 
        {
           if(GetAngle() > crossed->GetAngle()) newangle= (-GetAngle()+crossed->GetAngle())/2;
           else newangle= (GetAngle()-crossed->GetAngle())/2;
        }
        else newangle= (GetAngle()+crossed->GetAngle())/2;
       }
       
       else
       {
        if(GetAngle() > 0 && crossed->GetAngle() > 0 || GetAngle() < 0 && crossed->GetAngle() < 0)
          newangle= (GetAngle()+crossed->GetAngle())/2;
        else newangle= 180+(GetAngle()+crossed->GetAngle())/2;
       }
       //Starken Laser mit dem neuen Winkel setzen, Beide verursachende Laserobjekte übergeben
       newlaser->Set(newangle, (GetWidth()+crossed->GetWidth())*2/3, iMaxDist, iTime, 0, 0, 0, this, crossed);
       if(newlaser) //Wurde der starke Laser überhaupt erstellt?
       {
       newlaser->SetClrModulation((GetClrModulation()+crossed->GetClrModulation())/2);
       newlaser->SetContinuous(3);
       newlaser->SetObjectBlitMode(0);
       }
       //Message(Format("%d",newangle),this);
      }    
      LocalN("Overcharge",crossed)= 0; 
      Overcharge= 0;
	  }
	  if(newlaser)
      {
      var newangle;
      newangle= 0;
       if(Inside(GetAngle(),-90,90) && Inside(crossed->GetAngle(),-90,90))
       {
        //Message("1",this);
        if(GetAngle() > 0 && crossed->GetAngle() > 0 || GetAngle() < 0 && crossed->GetAngle() < 0)
          newangle= (GetAngle()+crossed->GetAngle())/2; //Dieser Winkel wäre der zu erwartende, aber da kann man nicht soo viel am Boden treffen 
          //newangle= (GetAngle()+crossed->GetAngle());
        else newangle= (GetAngle()+crossed->GetAngle())/2;
       }
       else if((!Inside(GetAngle(),-90,90) && Inside(crossed->GetAngle(),-90,90))||(Inside(GetAngle(),-90,90) && !Inside(crossed->GetAngle(),-90,90)))
       {
        //Message("2",this);
        //if(GetAngle() > 0 && crossed->GetAngle() > 0 || GetAngle() < 0 && crossed->GetAngle() < 0)
        //  newangle= (GetAngle()+crossed->GetAngle())/2; //Dieser Winkel wäre der zu erwartende, aber da kann man nicht soo viel am Boden treffen 
        if((Abs(GetAngle()) + Abs(crossed->GetAngle())) > 180)
        {
          if(GetAngle() > 0 && crossed->GetAngle() < 0 || GetAngle() < 0 && crossed->GetAngle() > 0)
          {
          if(Abs(GetAngle()) > Abs(crossed->GetAngle()))
             newangle= (-GetAngle()+crossed->GetAngle())/2;
          else newangle= (GetAngle()-crossed->GetAngle())/2;
          }
          else newangle= (GetAngle()+crossed->GetAngle())/2;
        }
          //newangle= (GetAngle()+crossed->GetAngle());
        else newangle= (GetAngle()+crossed->GetAngle())/2;
       }
       
       else
       {
        //Message("3",this);
        if(GetAngle() > 0 && crossed->GetAngle() > 0 || GetAngle() < 0 && crossed->GetAngle() < 0)
          newangle= (GetAngle()+crossed->GetAngle())/2;
        //else newangle= 180+ GetAngle()+crossed->GetAngle();
        else newangle= 180+(GetAngle()+crossed->GetAngle())/2;
       }
	    newlaser-> SetAngle(newangle);
	    SetPosition(mx,my,newlaser);
	    }
	}
  //if(crossed&& iElement == LocalN("iElement",crossed) && iElementar == LocalN("iElementar",crossed)) 
  else if(crossed)
  {
    if(newlaser) {RemoveObject(newlaser); Overcharge = 0;};
    CreateParticle("Terashock",AbsX(mx),AbsY(my),0,0,Overcharge*24,(GetClrModulation()+crossed->GetClrModulation())/2); 
    Overcharge++; 
    if(Overcharge>15)
    { 
      CreateObject(_BCR,AbsX(mx),AbsY(my))-> Explode(80);
      LocalN("pAttach",crossed)->ControlThrow();
      //Fling(LocalN("pAttach",crossed),0,-3);
      RemoveObject(crossed);
      //Fling(pAttach,0,-3);
      pAttach->ControlThrow();
      RemoveObject();
      return(0);
    }
  }
  else
  {if(newlaser) RemoveObject(newlaser);}
  //else{ if(newlaser) RemoveObject(newlaser);}
  //CreateObject(_BCR,AbsX(mx),AbsY(my));
  aData=[];
  
  //crossed= false;
  //xs= 0; ys= 0;
  /*if(pAttach)
    var xattach= GetX(pAttach)-GetX(), yattach= GetY(pAttach)-GetY();   
    while(++ml) {
    if(GBackSolid(mx*ml/iMaxDist,my*ml/iMaxDist)) break;
    if(FindObject2(Find_Not(Find_Distance(15,xattach,yattach)),Find_Distance(20,mx*ml/iMaxDist,my*ml/iMaxDist), Find_Exclude(pAttach), Find_Exclude(this), Find_OCF(OCF_Alive)))break;
    if(ml>=iMaxDist) break;
  }*/
  // graphic
  //SetLaser(ml == iMaxDist);
  SetLaser(0);
  l = 1000*ml/GetActMapVal("Facet",GetAction(),0,3);
  LaserEnd(mx,my);
}

private func DamageTarget()
{
  // no hit check
  if(!pDmgCallback)
	return(0);

  // clonk = user of the weapon that fires (the shooter). Will be given with clonk
  var clonk;
  clonk = pDmgCallback->~GetUser();
  if(!clonk) clonk = pDmgCallback;
	
	
	var temp_l, hitobject;
	
  // search points on the line for clonks etc.
  // step for step
  for(pObj in FindObjects(Find_Distance(30,mx,my), Sort_Distance(0,0),
              Find_Exclude(this()),
              Find_Exclude(pAttach),
              Find_Exclude(pDmgCallback),
              Find_Exclude(Contained(pDmgCallback)),
              Find_NoContainer(),
              Find_Or(
              	Find_Func("IsBulletTarget"),
              	Find_OCF(OCF_Alive)),
              Find_Func("CheckEnemy",this())))
  {
    if(pObj == LocalN("pAttach",crossed)) continue;
  	// we have to save this before, if the object is removed on hit
  	temp_l = 1000*ObjectDistance(pObj)/GetActMapVal("Facet",GetAction(),0,3);;
  	if(!iTime)
  	if(pDmgCallback ->~ LaserStrike(pObj,c))
  	{
  		//SetLaser(0);
  		l = temp_l;
  		hitobject = true;
  		break;
  	}
  }
  //if(ml != iMaxDist && reflections && !hitobject && !Stuck())
	//	CheckGround(ml);
}


private func NotZero(int a) {
  if(!a) return(1);
  else return(a);
}

private func SetLaser(bool end) {
  if(end) {
    if(GetAction() != "LaserEnd") {
      SetAction("LaserEnd");
    }
  }
  else {
    if(GetAction() != "Laser") {
      SetAction("Laser");
    }
  }
}

protected func Laser() {		// fade out
  // attach to obj
  if(pAttach) {
		var wx,wy;
		pAttach->~WeaponEnd(wx,wy);
		if(wx || wy)
			SetPosition(GetX(pAttach) + wx, GetY(pAttach) + wy);
		else
	    SetPosition(GetX(pAttach)+dx,GetY(pAttach)+dy);
	   
		SetPosition(GetX(),GetY(),pLight);
  }
  if(!pDmgCallback || !pAttach) RemoveObject();
  // animation & effect
  SetPhase(Random(4));
  if(GetAction() == "Laser" && !reflections) {
    var x,y;
    LaserEnd(x,y);
    var color = GetClrModulation();
    if(!crossed)
    if(pObstacle|| wallhit)
    //LaserLight(20+GetWidth()*2,color,x,y);
    if(!xs&&!ys) 
    {
      LaserLight(20+GetWidth()*2,color,x,y);
      var a,r,g,b;
      SplitRGBaValue(color,r,g,b,a);
      a = BoundBy(a+64,180,255);
      CreateParticle("PSpark",x,y,0,0,300+GetWidth()*30,RGBa(r,g,b,a));
      if(pObstacle)
      {
         var target= pObstacle;
         var containedtarget= Contained(target);
         if(containedtarget) target = containedtarget;
         DoDamage(1,target);
         DoEnergy(-1,target,0,0,GetOwner(this)+1);
      }
      if(wallhit)
      {
      //if(wallhitX == x && wallhitY == y) 
      if(Inside(x,wallhitX-3,wallhitX+3) && Inside(y,wallhitY-3,wallhitY+3))
      {
        CreateParticle("Terashock",x,y,0,0,MiningCharge*4,GetClrModulation()); 
        MiningCharge++; 
		if(GetMaterial(x,y) == Material("Vehicle") && FindObject2(Find_Func("IsOreDeposit"),Find_AtPoint(x,y))){
			MiningCharge = RandomX(5,20);
			Sound("Drill");
			FindObject2(Find_Func("IsOreDeposit"),Find_AtPoint(x,y))->GetMined(this(),1);
		}else
        if(MiningCharge>60) 
        {
         //CreateObject(_BCR,x,y)-> Explode(20);
         BlastFree(x+GetX(),y+GetY(), 20, GetOwner(this)+1);
         MiningCharge=0;
         Sound("MiningLaserBlast");
        }
      }      
      else {wallhitX = x; wallhitY = y; MiningCharge=0;}
      if(!wallhitX) wallhitX=x;
      if(!wallhitY) wallhitY=y; 
      }
    }
  }

  if(!iTime) return(0);

  // fade out (only if time != 0)
  var a = 200*c/iTime;
  var rgba = SetRGBaValue(GetClrModulation(),a,0);
  SetClrModulation(rgba);
  ++c;

  if(c >= iTime)
    RemoveObject();
}

protected func Destruction() {
  if(pLight) RemoveObject(pLight);
  if(newlaser) RemoveObject(newlaser);
}


private func DrawTransform() {		// draw line

  var cl = l;
  cl = l*GetActMapVal("Facet","Laser",0,3)/GetActMapVal("Facet",GetAction(),0,3);

  var fsin = -Sin(r, 1000), fcos = Cos(r, 1000);

  var xoff = -(GetActMapVal("Facet",GetAction(),0,2)*w/1000)/2;
  var yoff = 0;

  var width = +fcos*w/1000, height = +fcos*cl/1000;
  var xskew = +fsin*cl/1000, yskew = -fsin*w/1000;

  var xadjust = +fcos*xoff + fsin*yoff;
  var yadjust = -fsin*xoff + fcos*yoff;

  // set matrix values
  SetObjDrawTransform (
    width, xskew, xadjust,
    yskew, height, yadjust
  );
  
  // light
  if(pLight) {
    var alphamod, sizemod;
    CalcLight(alphamod, sizemod);

    var light_width = w*(5+sizemod/100);
    var light_height = cl/2;
    xoff = -(GetActMapVal("Facet","Laser",LALI,2)*light_width/1000)/2;
    xadjust = +fcos*xoff + fsin*yoff;
    yadjust = -fsin*xoff + fcos*yoff;
    width =  +fcos*light_width/1000;
    height = +fcos*light_height/1000;
    xskew =  +fsin*light_height/1000;
    yskew =  -fsin*light_width/1000;
    
    var a,r_,g,b;
    SplitRGBaValue(GetClrModulation(),r_,g,b,a);
    SetClrModulation(RGBa(r_,g,b,Min(a+160+alphamod,180)),pLight);
    SetPosition(GetX(),GetY(),pLight);
    
    SetObjDrawTransform (
      width, xskew, xadjust,
      yskew, height, yadjust, pLight
    );
  }
}

private func FxBeamCheckTimer(object target, int effect, int time) {
  // ein bischen Hax: wenn der Laser bleibt, wird auch der Winkel
  // angepasst, falls der Target AimAngle hat (Clonk hat das)
  var angle;
  if(pAttach) {
      if(crossed)
      angle = (pAttach->~AimAngle(0));
      else
      angle = (pAttach->~AimAngle(5));
      SetAngle(angle);
      if((GetDir(pAttach) == DIR_Right) && (dx < 0)) dx *= -1;
      if((GetDir(pAttach) == DIR_Left) && (dx > 1))  dx *= -1;
    }
  if(crossed) crossed-> HitCheck();
  HitCheck();
    //HitCheck();
    DrawTransform();
    //DamageTarget();
}

private func FxContinuousTimer(object target, int effect, int time) {
    DamageTarget();
}


// uber-cool reflection script
func CheckGround(int length)
{

	var x,y;
	LaserEnd(x,y);
	
	var r = Normalize(LocalN("r"),0);
	var angle1, angle2; // Winkel des Materials
	var angle1set, angle2set;
	var rout; //ausgangswinkel
	var i;

	var oldx = x, oldy = y;
	// Adjust position a little bit
	for(i = 0; oldx == x && oldy == y; i++)
	{
		x = oldx+Sin(r, i);
		y = oldy-Cos(r, i);
	}

	// Search for the angle of the solid material
	for(i = 0; i < 180; i++)
	{
		if(GBackSolid(x+Sin(r+i,5), y-Cos(r+i,5)) && !angle1set)
		{
			angle1set = true;
		}
		if(GBackSolid(x+Sin(r-i,5), y-Cos(r-i,5)) && !angle2set)
		{
			angle2set = true;
		}	
		if(!angle2set)
		{
			angle1 = r-i;
		}
		if(!angle1set)
		{
			angle2 = r+i;
		}
	}

	//nichts gefunden? wir müssen im Material feststecken.
	if(!angle1set || !angle2set)
		return(0);
	
	var angle = Normalize(Angle(Sin(angle1,5),-Cos(angle1,5),Sin(angle2,5),-Cos(angle2,5)),0);
	
	// calculate the angle of the new laser
	rout = Normalize(angle*2 - (r+180),0);

	oldx = x; oldy = y;
	// adjust position
	for(i = 0; GBackSolid(x,y); i++)
	{
		x = oldx+Sin(rout, i);
		y = oldy-Cos(rout, i);
	}

	// create new laser
	var newlaser = CreateObject(GetID(), x,y, GetOwner());
	if(newlaser)
	{
	newlaser->SetClrModulation(GetClrModulation());
	newlaser->Set(rout, GetWidth(), iMaxDist-length, iTime, pDmgCallback, 0/*pAttach*/, reflections-1);
  }
}

public func GetUser()
{
  return(pAttach);
}

func IsBulletTarget() { return(0); }

func HasObstacle() { return(pObstacle); }

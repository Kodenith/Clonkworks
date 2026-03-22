#strict

public func Activate(caster, real_caster) {

  //particles
  var iRange = 150;
  var ComboObj;
  if(ComboObj = Contents(0,caster)){
    if(GetID(ComboObj) == ADM3){
      RemoveObject(ComboObj);
      iRange = 250;
    }
  }
   var iAng, iX, iY, iRng;
  for(var i=iRange/2; i>0; --i)
  {
    iAng=Random(360);
    iRng=Random(iRange/2);
    iX=Sin(iAng, iRng);
    iY=Cos(iAng, iRng);
    CreateParticle("PSpark", iX, iY, RandomX(-5,5), RandomX(-5,5), 20+Random(65),RGBa(0,RandomX(255,150),RandomX(255,200)));
  }  

  Sound("Electric");

  	for(var i in FindObjects(Find_Distance(iRange))){
		  i->~Malfunction();
	  }
  
  RemoveObject();
  return(1);
}


public func GetSpellClass(object pMage) { return(WATR); }
public func GetSpellCombo(pMage) { return ("363"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)

#strict 2

global func Chance(int chance) { // chance from 0 to 100%
	return Random(100) < chance;
}

global func Normalize(int iAngle, int iStart, int iPrecision) {
  if(!iPrecision) iPrecision = 1;
  var iEnd = iPrecision*360+iStart;
  
  while(iAngle<iStart) iAngle += iPrecision*360;
  while(iAngle>=iEnd) iAngle -= iPrecision*360;

  return iAngle;
}

global func CheckVisibility(object Obj1, object Obj2) {	// if Owner of Obj2 can see Obj1
	if(!Obj2)
		Obj2 = this;
		
	var vis = GetVisibility(Obj1);
	if(vis != VIS_All) {
		var own1 = GetOwner(Obj1);
		var own2 = GetOwner(Obj2);
	
		// VIS Owner
		if(vis & VIS_Owner)
			if(own1 == own2)
				return true;
		// VIS Enemies
		if(vis & VIS_Enemies)
			if(Hostile(own1,own2))
				return true;
		// VIS Allies
		if(vis & VIS_Allies)
			if(!Hostile(own1,own2))
				return true;
		// VIS Local
		if(vis & VIS_Local) {
			var lo = own2/32;
			var bit = 1 << own2%32;
			// man, kompliziert...
			if(bit & Local(lo,Obj1))
				return true;
		}
		// sonst gibt es keine Modi
		return false;
	}
	return true;
}

global func GetShooter(object weapon) {

  var shooter;

  if(!weapon) return;
  
  // Waffe?
  if(weapon->~IsWeapon()) {
    shooter = weapon->GetUser();
	// Schütze ist irgendwo drin
	if(Contained(shooter))
	  shooter = Contained(shooter);
	//oder fässt was an
	if(shooter)
	  if(shooter->GetActionTarget())
	    shooter = GetActionTarget();
  }
  
  // noch immer nichst gefunden?
  if(!shooter)
    // dann einfach weapon nehmen.
    shooter = weapon;
	
	return shooter;
}

global func AngleDiff(a1,a2,p) {
	if(!p) p=1;
	a1 %= 360*p; a2 %= 360*p;
	while(a1<0) a1+=360*p;
	while(a2<0) a2+=360*p;
	var r1 = Normalize(a2-a1,-180*p,p);
	var r2 = Normalize(a2-a1-360*p,-180*p,p);
	if(Abs(r1) < Abs(r2)) return(r1);
	else return(r2);
}

/* Partikeleffekte */

#strict

global func Sparks(int iAmount, int iRGB, int iX, int iY)
{
  // Partikel verschleudern
  var iAngle, iSpeed, iSize, iLevel=30;
  for( var i=0; i<iAmount; ++i)
  {
    iAngle = Random(360);
    iSpeed = RandomX(0,iLevel);
    iSize = RandomX(25,40);
    CreateParticle("Frazzle",iX+RandomX(-2,+2),iY+RandomX(-2,+2),Sin(iAngle,iSpeed),Cos(iAngle,iSpeed),iSize,iRGB);
  }
}

global func LaserLight(int iSize, int iRGB, int iX, int iY, int iIterations)
{
  if(!iIterations) iIterations = 1;

  for(var i = 0; i < iIterations; ++i) {
    var random = Random(360);
    CreateParticle("LsrSprk",iX,iY,Sin(random,10),-Cos(random,10),iSize*5,iRGB);
  }
  
  if(!IsDark())
    return();
  
  if(!iRGB)
    iRGB = RGB(255,255,255);

  var r,g,b,a;
  SplitRGBaValue(iRGB,r,g,b,a);
  iRGB = RGBa(r,g,b,Min(a+65,255));

  //AddLightFlash(iSize*14, iX, iY, iRGB);

}

global func BulletCasing(int iX, int iY, int iXDir, int iYDir, int iSize, int iColor) {
  CreateParticle("BulletCasing",iX,iY,iXDir,iYDir,iSize*5,iColor);
}

/* Damage Effect */

global func AddDamageEffect(object target, int size) {
	// target muss die Funktion  GetDmgEffectPos(&x,&y) definiert haben
	// und MaxDamage
	if(!target) target = this;
	if(!target) return;
	if(!size) size = 100;
	
	var eff = AddEffect("DamageEffect", target, 1, 1, 0, 0); 
  EffectVar(0,target,eff) = size;
}

global func FxDamageEffectTimer(object target, int number, int time) {
	// wenn noch nicht ausreichend besch digt, lassen
	var dmg = target->GetDamage();
	var maxdmg = target->~MaxDamage();
	var size = EffectVar(0,target,number);

	if(dmg < maxdmg/2) return;
	if(target->Contained()) return;

	var x, y;
	target->~GetDmgEffectPos(x,y);
	
	// occupacy of the smoke is dependend on  damage taken
	var xdir = +RandomX(-10,10);
	var ydir = -RandomX(-15,10);
	var smoke = RGBa(220,180,110,BoundBy(maxdmg-dmg,20,255));
	var thrust = RGBa(255,200,200,BoundBy(maxdmg-dmg,40,255));

	CreateParticle("Smoke2",target->GetX()+x,target->GetY()+y,xdir,ydir,size*RandomX(80,380)/100,smoke);
	CreateParticle("Thrust",target->GetX()+x,target->GetY()+y,xdir,ydir,size*RandomX(120,200)/100,thrust);
}

/* Spawn-Effekt */

global func AddSpawnEffect(object pObj, int rgba)
{
  if(!pObj)
    if(!(pObj = this()))
      return();
  AddEffect("Spawn", pObj, 5, 1, 0, 0, rgba);
}

global func FxSpawnStart(object pTarget, int iNo, int iTemp, int rgba)
{
  if(iTemp) return();
  EffectVar(0, pTarget, iNo) = rgba;
  EffectVar(1, pTarget, iNo) = 360;
}

global func FxSpawnTimer(object pTarget, int iNo)
{
  var rgba = EffectVar(0, pTarget, iNo);
  var a = EffectVar(1, pTarget, iNo);
  if(EffectVar(1, pTarget, iNo) <= 0)
  {
    CastParticles("PSpark", 20, 50, GetX(pTarget), GetY(pTarget), 10, 50, rgba, rgba);
    return(-1);
  }
  for(var i=0, x,y ; i < 5 ; i++)
  {
    x = Cos(a - 90 * i, a / 20);
    y = Sin(a - 90 * i, a / 20);
    CreateParticle("PSpark", GetX(pTarget) + x, GetY(pTarget) + y, 0,0, BoundBy(((360 - a) / 10) * 5, 5, 25), rgba, pTarget);
  }

  EffectVar(1, pTarget, iNo)-=10;
}

/* Checkt ob hier ein Dust-Partikel erstellt werden darf */

global func CheckDust(int mat) {

		// check if digable
		var digable = GetMaterialVal("DigFree","Material",mat);
		
		// hack for not-digable materials but should be by name digable
		if(!digable) {
			var name = GetMaterialVal("Name","Material",mat);
			if(WildcardMatch(name, "*Earth")) digable = true;
			else if(WildcardMatch(name, "*Sand")) digable = true;
			else if(WildcardMatch(name, "*Ashes")) digable = true;
			else if(WildcardMatch(name, "*Snow")) digable = true;
		}
		
		return digable;
}

global func VehicleSpawnEffect(object pObj)
{
	if(!pObj) return;
	var size = Sqrt(Pow(pObj->GetDefWidth(),2)+Pow(pObj->GetDefHeight(),2));
	var color = RGBa(0,200,255,100);
	pObj->CastParticles("PSpark", 30, 80, 0, 0, size, size*2, color, color);
}
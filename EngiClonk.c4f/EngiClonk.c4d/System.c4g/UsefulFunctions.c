/*-- Neues Script --*/

#strict 3

//Returns a random color
//Include Alpha - Alpha is also random
global func RandomRGBa(bool IncludeAlpha){
	if(IncludeAlpha) return(RGBa( RandomX(0,255) , RandomX(0,255) , RandomX(0,255) , RandomX(0,255)));
	return(RGBa( RandomX(0,255) , RandomX(0,255) , RandomX(0,255)));
}

//Calculates and bounces an object based on current velocity, and if its touching a wall. can be used to make bouncy objects
//pObject - The object being bounced, can be 0 in local calls
//Power - how powerful is the bounce? numbers past 10 make the object bounce without end and eventually reach extreme speeds!
//xdir - current x velocity, Pass in xdir from Hit, Hit2, or Hit3
//ydir - current y velocity, Pass in ydir from Hit, Hit2, or Hit3
// you can pass in anything to xdir and ydir, but you might need to make the numbers big
global func HitBounce(object pObject,int Power, int xdir, int ydir){
	    var Devide = 20;
		Devide -= Power;
		var wdt = GetDefCoreVal("Width", "DefCore", GetID(pObject));
		if(GetXDir(pObject) == 0 && GetYDir() == 0) return(0);
		if(Stuck(pObject)) return(0);
		//if on cooldown, dont bounce.
		if(GetEffect("HBCooldown",pObject)) return(0);
	
		var HitWall;
		//check if touching wall
		HitWall = false;
		if(xdir > 0){
			if(pObject->GBackSolid(2+wdt,-2)) HitWall = true;
		}else{
			if(pObject->GBackSolid(-2-wdt,-2)) HitWall = true;
		}
		
		if(xdir > 50) xdir = 50;
		if(xdir < -50) xdir = -50;
		
		if(Abs(xdir) != 0){
			//if wall is hit, swap x velocity. if not the continue but slow down
			if(HitWall) SetXDir((xdir/Devide), pObject);
				else SetXDir(-(xdir/Devide)*2, pObject);
		}
		
		if(Abs(ydir) != 0){
			if(!HitWall)
				SetYDir(-(ydir/Devide), pObject);
			else SetYDir((ydir/Devide), pObject);
			if(Abs(xdir) > 0 && !HitWall) SetXDir(GetXDir()+xdir/2, pObject);
			
			if(HitWall && xdir < 0) SetXDir(xdir, pObject);
			if(HitWall && xdir > 0) SetXDir(-xdir, pObject);
		}
		
		AddEffect("HBCooldown", pObject, 50, 5, , );
		
		return(1);
}

global func FxHBCooldownStop(object pTarget, int iEffectNumber, int iReason, bool fTemp){
	//once this effect is finished the object can bounce again.
}

//this function allows the creation of forged objects without the forge itself, useful for minigames or spawning a preforged item
//if an material is not on the forge list or the Forging Allowed rule is not in the scenario, this returns 0
//also, if the object you wanna spawn isnt forgeable, this will also return 0.
global func CreateForgedObject(id Id, int XOffset, int YOffset, int Owner, id MaterialId){
	var ListParent = FindObject2(Find_ID(FALW));
	var ok = false;
	var Index;
	
	if(!ListParent) return(0);
	
	for(var checkId = 0; checkId < GetLength(LocalN("ForgeableIDs", ListParent)); checkId++){
		if(LocalN("ForgeableIDs", ListParent)[checkId] == MaterialId){
			ok = true;
			Index = checkId;
			continue;
		}
	}
	
	if(!ok) return(0);
	if(!GetComponent(DUMM,,,Id)) return(0);
	
	var ForgedObj = CreateObject(Id, XOffset, YOffset, Owner);
	LocalN("Color", ForgedObj) = LocalN("ForgeableColors", ListParent)[Index];
	LocalN("Mass", ForgedObj) = LocalN("ForgeableMass", ListParent)[Index];
	LocalN("Speed", ForgedObj) = LocalN("ForgeableSpeed", ListParent)[Index];
	LocalN("Power", ForgedObj) = LocalN("ForgeablePower", ListParent)[Index];
	LocalN("Effect", ForgedObj) = LocalN("ForgeableEffect", ListParent)[Index];
	LocalN("MatId", ForgedObj) = MaterialId;
	
	ForgedObj->FRGUpdate();
	ForgedObj->AssignEffects();
	
	return(ForgedObj);
}

//same as above, but it creates a custom forge object with values passed into the function, instead of using a forge material. Not inspectable.
global func CreateForgedObjectCustom(id Id, int XOffset, int YOffset, int Owner, int Color, int Mass, int Speed, int Power, string Effect){
	if(!GetComponent(DUMM,,,Id)) return(0);
	
	var ForgedObj = CreateObject(Id, XOffset, YOffset, Owner);
	LocalN("Color", ForgedObj) = Color;
	LocalN("Mass", ForgedObj) = Mass;
	LocalN("Speed", ForgedObj) = Speed;
	LocalN("Power", ForgedObj) = Power;
	LocalN("Effect", ForgedObj) = Effect;
	LocalN("MatId", ForgedObj) = 0;
	
	ForgedObj->FRGUpdate();
	ForgedObj->AssignEffects();
	
	return(ForgedObj);
}

//This function draws a particle line similiar to a line kit. it will bend on walls.
//requires strict 3
global func DrawParticleWire(string ParticleType,int gX,int gY,object cnt, int Color1, int Color2){
var path = GetPath(GetX(), GetY(), GetX(cnt), GetY(cnt));
if(!path) return(0);
for(var xymap in path.Waypoints){
	if(xymap["TransferTarget"]){
		//nothing
	}else{
	global->DrawParticleLine(ParticleType, gX, gY, xymap["X"], xymap["Y"], 2, 10, Color1, Color2, 0);
	gX = xymap.X;
	gY = xymap.Y;
	}
}
return(1);
}
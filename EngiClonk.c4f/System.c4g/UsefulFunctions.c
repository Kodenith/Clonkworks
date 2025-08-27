/*-- Neues Script --*/

#strict 2

//Returns a random color
//Include Alpha - Alpha is also random
global func RandomRGBa(bool IncludeAlpha){
	if(IncludeAlpha) return(RGBa( RandomX(0,255) , RandomX(0,255) , RandomX(0,255) , RandomX(0,255)));
	return(RGBa( RandomX(0,255) , RandomX(0,255) , RandomX(0,255)));
}

//Calculates and bounces an object based on current velocity, and if its touching a wall. can be used to make bouncy objects
//pObject - The object being bounced, can be 0 in local calls
//Devide - How much to devide from the bounciness? 1-5 Not much, 6-10+ a lot, Any Negative number makes it bounce higher and higher without stopping!
//xdir - current x velocity, Pass in xdir from Hit, Hit2, or Hit3
//ydir - current y velocity, Pass in ydir from Hit, Hit2, or Hit3
// you can pass in anything to xdir and ydir, but you might need to make the numbers big
global func HitBounce(object pObject,int Devide, int xdir, int ydir){
	    Devide += 10;
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
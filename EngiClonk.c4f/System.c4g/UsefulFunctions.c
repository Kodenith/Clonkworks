/*-- Neues Script --*/

#strict 2

//Returns a random color
global func RandomRGBa(bool IncludeAlpha){
	if(IncludeAlpha) return(RGBa( RandomX(0,255) , RandomX(0,255) , RandomX(0,255) , RandomX(0,255)));
	return(RGBa( RandomX(0,255) , RandomX(0,255) , RandomX(0,255)));
}

//Makes Objects Bouncy
global func HitBounce(int Devide, int xdir, int ydir){
		var wdt = GetDefCoreVal("Width", "DefCore", GetID());
		if(GetXDir() == 0 && GetYDir() == 0) return(0);
		if(Stuck()) return(0);
		//if on cooldown, dont bounce.
		if(GetEffect("HBCooldown",this())) return(0);
	
		var HitWall;
		//check if touching wall
		HitWall = false;
		if(xdir > 0){
			if(GBackSolid(2+wdt,-2)) HitWall = true;
		}else{
			if(GBackSolid(-2-wdt,-2)) HitWall = true;
		}
		
		if(xdir > 50) xdir = 20;
		if(xdir < -50) xdir = -20;
		
		if(Abs(xdir) != 0){
			//if wall is hit, swap x velocity. if not the continue but slow down
			if(HitWall) SetXDir((xdir/Devide));
				else SetXDir(-(xdir/Devide)*2);
		}
		
		if(Abs(ydir) != 0){
			if(!HitWall)
				SetYDir(-(ydir/Devide));
			else SetYDir((ydir/Devide));
			if(Abs(xdir) > 0 && !HitWall) SetXDir(GetXDir()+xdir/2);
			
			if(HitWall && xdir < 0) SetXDir(xdir);
			if(HitWall && xdir > 0) SetXDir(-xdir);
		}
		
		AddEffect("HBCooldown", this(), 50, 5, , );
		
		return(1);
}

global func FxHBCooldownStop(object pTarget, int iEffectNumber, int iReason, bool fTemp){
	//once this effect is finished the object can bounce again.
}
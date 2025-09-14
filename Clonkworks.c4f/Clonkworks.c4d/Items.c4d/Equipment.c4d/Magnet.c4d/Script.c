#strict 2

func Initialize() {
  
  return(1);
}

func Attract(){
	var atrc = FindObjects(Find_Func("IsMetal"), Find_Distance(50), Find_Exclude(this()));
	var MaxAtr = 10;
	for(var metl in atrc){
		if(ObjectDistance(metl, this()) <= 10) continue;
		if(GetX(metl) > GetX(this())) SetXDir(GetXDir(metl)-RandomX(3,MaxAtr), metl);
		if(GetX(metl) < GetX(this())) SetXDir(GetXDir(metl)+RandomX(3,MaxAtr), metl);
		if(GetY(metl) > GetY(this())) SetYDir(GetYDir(metl)-RandomX(MaxAtr/2,MaxAtr), metl);
		if(GetY(metl) < GetY(this())) SetYDir(GetXDir(metl)+RandomX(MaxAtr/2,MaxAtr), metl);
	}
}

func IsMetal(){ return(true); }
func IsAnvilProduct(){ return(1); }
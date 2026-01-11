#strict 2

local LiquidType;
local Amount;

func Initialize(){
	AddEffect("LiquidStorageUpdate",this(),1,1,this());
}
func MaxAmount(){ return(1000); }

func FxLiquidStorageUpdate(pTarget, iNum, iTime){
	if(!LiquidType) Amount = 0;
	if(Material(LiquidType) == -1) Amount = 0;
	if(Amount > MaxAmount()) Amount = MaxAmount();
	if(Amount < 0) Amount = 0;
	if(Amount == 0) LiquidType = 0;
	
	SetMass(GetMass(,GetID())+(Amount/3));
}

public func AcceptTransfer(){
	if(Amount < MaxAmount()) return(1);
}

//inserting and removing materials inside
public func InsertLiquidPx(string Type, int iAmount){
	if(iAmount < 0) return(0);
	if(Type == "Sky") return(0);
	if(!LiquidType){
		LiquidType = Type;
	}
	
	for(var i = 0; i < iAmount; i++){
		if(Type != LiquidType || Amount >= MaxAmount()){
			InsertMaterial(Material(Type));
		}else{
			Amount++;
		}
	}
	
	return(Amount);
}

public func OutsertLiquidPx(int iAmount){
	for(var i = 0; i < iAmount; i++){
		if(Amount <= 0) return(i);
		InsertMaterial(Material(LiquidType));
		Amount--;
	}
}

public func SetLiquidType(string Type){
	LiquidType= Type;
}

public func SetLiquidAmount(int am){
	Amount = am;
	if(Amount > MaxAmount()) Amount = MaxAmount();
	if(Amount < 0) Amount = 0;
}

public func GetLiquidType(){ return(LiquidType); }
public func GetLiquidAmount(){ return(Amount); }

public func IsLiquidStorage(){ return(1); }

func HasLiquid(){ return(Amount > 0); }
func IsFull(){ return(Amount >= MaxAmount()); }

global func BarrelList(){
	var b,j,l;
	l = [];
	while(b = MaterialName(j++)){
		var br = GetBarrelType(Material(b));
		if(br) ArrayAdd(l,br,true);
	}
	return(l);
}
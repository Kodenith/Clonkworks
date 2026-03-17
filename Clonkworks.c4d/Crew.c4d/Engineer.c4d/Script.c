#strict 3
#include CLNK

private func MaxContentsCount() { return(2); } 

protected func ControlSpecial() 
{ 
  [$TxtShift$|Image=CXIV]
  return (ShiftContents(nil,nil,nil,1)); 
} 

public func ContextEngiProduction(pClonk){
    [$TxtEngi$|Image=WRNC]
    if(pClonk == this) return(EngiMenu());
}

public func ControlSpecial2(){
    [$TxtEngi$|Image=WRNC]
    return(EngiMenu());
}

local Product;
local ComponentsArray;
local ComponentsArrayUsed;
local ComponentsArrayString;
local Size;

public func EngiMenu(){
    if(GetAction() == "Weld"){
        SetAction("KneelUp");
        return(1);
    }

    if(!Product){
        //item prod selection menu
        CreateMenu(WRNC,this,this,1);
        
        var i, x;
        while(x = GetDefinition(i++,C4D_Object)){
            if(x->~IsAnvilProduct() && GetPlrKnowledge(GetController(),x)){
                AddMenuItem("$TxtNewItem$","SetProduce",x,this,nil,nil,GetDesc(,x));
            }
        }

        i = 0;

        while(x = GetDefinition(i++,C4D_Vehicle)){
            if(IsVehicleNoCraftable(x)) continue;

            if(GetPlrKnowledge(GetController(),x)){
                AddMenuItem("$TxtNewItem$","SetProduce",x,this,nil,nil,GetDesc(,x));
            }
        }

        return(1);
    }else{
        if(Contents() && InArray(GetID(Contents()),ComponentsArray) != -1){
            var pId = GetID(Contents());
            RemoveObject(Contents());
            Sound("Grapple");
            ArrayDeleteEntry(ComponentsArray,pId);
            ArrayAdd(ComponentsArrayUsed,pId);
            SetComponentString();
            return(1);
        }else if(GetLength(ComponentsArray) == 0){
            CreateMenu(WRNC,this,this,0,"$TxtFinalMenu$",nil,1);
            AddMenuItem("$TxtNewItem$","BeginProduction",Product,this);
            AddMenuItem("$TxtFinalCancel$","CancelProduct",_MRK,this);
             return(1);
        }else{
            CreateMenu(WRNC,this,this,0,"$TxtCancel$",nil,1);
            AddMenuItem("$Y$","CancelProduct",_MRK,this,0,nil,"",2,1);
            AddMenuItem("$N$","Foo",_MRK,this);
             return(1);
        }
    }
}

private func BeginProduction(){
    if(GetAction() != "Walk" || Contained()){
        Sound("Error");
        PlayerMessage(GetController(),"$TxtCantProduce$",this,GetName());
        return(nil);
    }

    SetAction("KneelDown");
    ScheduleCall(this,"StartWeld",4);
}

private func StartWeld(){
    SetAction("Weld");
}

private func Welding(){
    if(GetActTime() == 1) Sound("Fuse");
    if(!Random(15)) Sound("Spark*");

    if(GetCategory(,Product) & C4D_Object){
        var WeldDir = -5;
        if(GetDir() == DIR_Right) WeldDir = 5;

        if(!Random(2))
        CastParticles("PxSpark",RandomX(1,5),RandomX(10,65),WeldDir-RandomX(-2,2),RandomX(6,10),10,50,RGBa(255,255,0),RGBa(255,0,0));

        if(GetActTime() > 38*4){
            SetAction("KneelUp");
            ComponentsArray = nil;
            ComponentsArrayString = nil;
            ComponentsArrayUsed = nil;

            if(ContentsCount() < 2) CreateContents(Product,this,1);
            else Exit(CreateContents(Product,this,1), WeldDir, GetDefBottom()-GetY());

            Product = nil;
        }
    }   

    if(GetCategory(,Product) & C4D_Vehicle){
        var Amount = RandomX(0,2);
        var OffsetX = GetDefWidth(Product)/2;
        var OffsetY = GetDefHeight(Product)/2;
        while(Amount--){
            CastParticles("PSpark",1,0,RandomX(-OffsetX,OffsetX),RandomX(-OffsetY,OffsetY),10,50,RGBa(255,255,0),RGBa(255,0,0));
        }

        if(GetActTime() > 38*10){
            SetAction("KneelUp");
            ComponentsArray = nil;
            ComponentsArrayString = nil;
            ComponentsArrayUsed = nil;

            Exit(CreateContents(Product,this,1), 0, GetDefBottom()-GetY());

            Product = nil;

            Amount = RandomX(10,45);
            while(Amount--){
            CastParticles("MSpark",1,RandomX(10,60),RandomX(-OffsetX,OffsetX),RandomX(-OffsetY,OffsetY),30,70,RGBa(255,255,0),RGBa(255,0,0));
            }

            Sound("MagicElementAir");
        }
    }   
}

private func CancelProduct(){
    for(var i in ComponentsArrayUsed){
        if(RandomX(0,1) == 1) Exit(CreateContents(i,this,1), 0, GetDefBottom()-GetY());
    }

    Product = nil;
    ComponentsArray = nil;
    ComponentsArrayString = nil;
    ComponentsArrayUsed = nil;

    Sound("Command");
    PlayerMessage(GetController(),"$TxtProductionCancel$",this);
}

private func Foo(){ return(0); }

private func SetProduce(ID){
    Sound("Ding");
    PlayerMessage(GetController(),"$TxtNewSet$",this,GetName(,ID));
    Product = ID;

    //setting components array.
    if(!ComponentsArray) ComponentsArray = [];
    ComponentsArrayString = "";
    ComponentsArrayUsed = [];

    var x, i;
    while(x = GetComponent(nil,i++,nil,ID)){
        var count = GetComponent(x,nil,nil,ID);
        while(count--){
            ArrayAdd(ComponentsArray,x);
        }
    }

    Size = GetLength(ComponentsArray);

    SetComponentString();
}

private func SetComponentString(){
    var Str = "";
    for(var x in ComponentsArray){
        Str ..= Format("{{%i}}",x);
    }
    ComponentsArrayString = Str;
    return(Str);
}

public func InfobarTrigger(){
    if(Product)
	    return(IB_Exist);
}

public func InfobarMax(){
	return(Size);
}

public func InfobarValue(){
	return(GetLength(ComponentsArrayUsed));
}

public func InfobarColor(){
    if(GetLength(ComponentsArray) != 0)
	    return(RGBa(255,255/2,0));
    else return(RGBa(0,255,0));
}

public func InfobarInfo(){
    if(GetLength(ComponentsArray) != 0)
	    return(Format("$IB1$",GetName(,Product),ComponentsArrayString));
    else return(Format("$IB2$",GetName(,Product)));
}

public func IsVehicleNoCraftable(pId){
    var x, i;
    while(x = GetComponent(nil,i++,nil,pId)){
        if(GetCategory(,x) & C4D_Vehicle) return(true);
    }

    return(false);
}
#strict 2
#include WOOD

public func Activate(pClonk){
    [$TxtRead$]
    if(GetCon() < 100) return(0);
    Sound("FlipPage");
    CreateMenu(GetID(),pClonk,this,1);
    AddMenuItem("$TxtAbout1$","ShowInfo",GetID(),pClonk,0,pClonk);
    AddMenuItem("$TxtCate1$","Automation",HLP0,pClonk,0,pClonk,,0,0);
    AddMenuItem("$TxtCate2$","Mining",HLP0,pClonk,0,pClonk,,2,1);
    AddMenuItem("$TxtCate3$","Wiring",HLP0,pClonk,0,pClonk,,2,2);
    AddMenuItem("$TxtCate4$","Other",HLP0,pClonk,0,pClonk,,2,3);
    return(1);
}

public func ShowInfo(pId,pClonk){
  Sound("FlipPage");
  MessageWindow(GetDesc(,pId), GetController(pClonk));
}

public func Array2Menu(Arr,pClonk){
    CreateMenu(GetID(),pClonk,this,1);
    for(var i in Arr){
        AddMenuItem("%s","ShowInfo",i,pClonk,0,pClonk);
    }
}

public func Automation(){
    var pClonk = Par(1);
    Sound("FlipPage");

    var Items = [];
    var i, x;
    while(x = GetDefinition(i++)){
        if(x->~PDI_Auto()){
            //if((GetCategory(,x) & C4D_Knowledge) && GetPlrKnowledge(GetController(pClonk),x) != true) continue;
            ArrayAdd(Items,x);
            if(x->~PDI_Children()){
                for(var j in x->~PDI_Children()){
                    if((GetCategory(,j) & C4D_Knowledge) && GetPlrKnowledge(GetController(pClonk),j) != true) continue;
                    ArrayAdd(Items,j);
                }
            }
        }
    }
    Array2Menu(Items,pClonk);
}

public func Mining(){
    var pClonk = Par(1);
    Sound("FlipPage");

    var Items = [];
    var i, x;
    while(x = GetDefinition(i++)){
        if(x->~PDI_Mine()){
            //if((GetCategory(,x) & C4D_Knowledge) && GetPlrKnowledge(GetController(pClonk),x) != true) continue;
            ArrayAdd(Items,x);
            if(x->~PDI_Children()){
                for(var j in x->~PDI_Children()){
                    if((GetCategory(,j) & C4D_Knowledge) && GetPlrKnowledge(GetController(pClonk),j) != true) continue;
                    ArrayAdd(Items,j);
                }
            }
        }
    }
    Array2Menu(Items,pClonk);
}

public func Other(){
    var pClonk = Par(1);
    Sound("FlipPage");

    var Items = [];
    var i, x;
    while(x = GetDefinition(i++)){
        if(x->~PDI_Othr()){
            //if((GetCategory(,x) & C4D_Knowledge) && GetPlrKnowledge(GetController(pClonk),x) != true) continue;
            ArrayAdd(Items,x);
            if(x->~PDI_Children()){
                for(var j in x->~PDI_Children()){
                    if((GetCategory(,j) & C4D_Knowledge) && GetPlrKnowledge(GetController(pClonk),j) != true) continue;
                    ArrayAdd(Items,j);
                }
            }
        }
    }
    Array2Menu(Items,pClonk);
}

public func Wiring(){
    var pClonk = Par(1);
    Sound("FlipPage");

    var Items = [];
    var i, x;
    while(x = GetDefinition(i++)){
        if(x->~PDI_Wire()){
            //if((GetCategory(,x) & C4D_Knowledge) && GetPlrKnowledge(GetController(pClonk),x) != true) continue;
            ArrayAdd(Items,x);
            if(x->~PDI_Children()){
                for(var j in x->~PDI_Children()){
                    if((GetCategory(,j) & C4D_Knowledge) && GetPlrKnowledge(GetController(pClonk),j) != true) continue;
                    ArrayAdd(Items,j);
                }
            }
        }
    }
    Array2Menu(Items,pClonk);
}
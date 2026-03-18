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

    var Items = [GRKT,WRNC,CNVY,ROTR,EXDR,MAM1,FTG1,FTG2,FTG3,FTG7,CNVS,ANV2,CHM2,SAW2,FND2,CHUB];
    Array2Menu(Items,pClonk);
}

public func Mining(){
    var pClonk = Par(1);
    Sound("FlipPage");

    var Items = [EXDR,MLCN,QUAR,CVCB,CFLN,LFLN,TFLS,STFS,MGNT,MTLD,HLP1];
    Array2Menu(Items,pClonk);
}

public func Other(){
    var pClonk = Par(1);
    Sound("FlipPage");

    var Items = [ECLK,CPIG,TTIG,GLUE,PRCS,MFLN,LOMB,TARP,ATRP,CK5P,CW5P,CT5P];
    Array2Menu(Items,pClonk);
}

public func Wiring(){
    var pClonk = Par(1);
    Sound("FlipPage");

    var Items = [WRKT,WRNC,CC1_,CC3_,CC4_,CC5_,I1_L,I1_P,MGAT,HATC,SPIK,SRIK,PIS1,I5_M];

    var CC1 = CC1_->UsedCircuits();
    ArrayAddArray(Items,CC1);

    var CC3 = CC3_->UsedCircuits();
    ArrayAddArray(Items,CC3);

    var CC4 = CC4_->UsedCircuits();
    ArrayAddArray(Items,CC4);

    var CC5 = CC5_->UsedCircuits();
    ArrayAddArray(Items,CC5);

    var Robs = [ELRY,EBOW,ECAN,CLBT,HLP2,HLP3];
    ArrayAddArray(Items,Robs);

    Array2Menu(Items,pClonk);
}
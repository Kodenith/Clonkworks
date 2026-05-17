#strict 2
func Hit(){
  Sound("Crystal*");
}

func IsAnvilProduct(){ return(1); }

public func DoSmoke(){
  Smoke(0,0,RandomX(5,8));
}
public func DoFuse(){
  Sound("Fuse");
}
public func DoBoom(){
  Explode(5);
}

public func Activate(pHaz){
  [$TxtUse$]
  if(GetID(pHaz) == HZMC && GetAction() == "Alright"){
    Sound("Connect");
    pHaz->SetAction("KneelDown");
    LocalN("Filter",pHaz) = BoundBy(LocalN("Filter",pHaz)+50,0,100);
    RemoveObject();
  }
}

public func RejectEntrance(){
  if(GetAction() == "Broken") return(1);
}

func Initialize(){ SetAction("Alright"); }
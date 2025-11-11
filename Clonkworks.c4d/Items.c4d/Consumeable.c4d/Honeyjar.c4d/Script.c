/*-- Neues Script --*/

#strict 2
#include ARWP

public func UnpackTo() { return(HONY); }
public func IsArrowPack() { return(0); }
public func MaxPackCount() { return(5); }
public func IsAnvilProduct() { return(0); }

public func Activate(pClonk){
	[$TxtDrink$]
	Sound("Drink");
	pClonk->Drink(this());
	DoEnergy(20,pClonk);
	
	DoPackCount(-1);
	
	UpdatePicture();
	UpdateMass();
}

public func CalcValue(object pInBase, int iForPlayer)
{
  // Je nach Anzahl
  return((GetValue(0, GetID())*PackCount()/MaxPackCount())+2);
}

public func DoPackCount(iChange) 
{
  if(iChange>0) Sound("OpenJar");
  iUsedItems-=iChange;
  if(PackCount()<=0) return(RemoveObject());
  UpdatePicture();
  // Masse anpassen
  UpdateMass();
}

public func Hit(){
	if(!Random(3)) Sound("Crystal*");
}

public func ContextUnpack(pClonk){
  [$TxtUnpack$|Image=HONY]
  return (Unpack(pClonk));
}

public func JoinPack(object pContainer)
{
  // In vorhandene Pfeilpakete integrieren
  var pObj, iChange, iAmount, iList;
  // Alle gleichen Pakete im Clonk durchsuchen
  iList = FindObjects(Find_Exclude(this()),Find_Container(pContainer),Find_ID(GetID()),Find_OCF(OCF_Fullcon));
  for(pObj in iList) {
    iAmount = PackCount();
    // Wie viel Platz ist im neuen Paket?
    iChange = MaxPackCount() - pObj->PackCount();
    // Wir können aber höchstens so viele abgeben wie wir haben
    if(iChange>iAmount) iChange = iAmount;
    // Pfeile abgeben
    pObj->DoPackCount(iChange);
    // Wenn das Paket leer ist aufhören
    if(iAmount-iChange<=0) return(RemoveObject());
    DoPackCount(-iChange);
  }
  return(0);
}
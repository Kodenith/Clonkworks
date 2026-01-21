/*-- Verallgemeinertes Brennstoffsystem für Kraftwerk, Hochofen, Herd und neue Gebäude --*/

#strict 2

local iStoredFuel;

//Ergänzung für Zappers arrays.c; fügt value an iPosition in aArray ein
global func InsertArrayItem(value, iPosition, &aArray)
{
 var temp=aArray;
 //blöde algorithmen+datenstrukturen-vorlesung... jetzt bin ich so performanceversessen
 for(var cnt = iPosition; cnt <= GetLength(aArray); cnt++)
 {
  if(cnt == iPosition) temp[cnt]=value;
  if(cnt >= iPosition && cnt < GetLength(aArray)) temp[cnt+1]=aArray[cnt];
 } 
 aArray=temp;
 return(aArray);
}	

//die Doku behauptet, dies würde existieren
global func InsertMaterialAmount(int iMat, int iAmount, int iX, int iY, int iXDir, int iYDir) {
 for(var i; i <= iAmount; i++)
  InsertMaterial (iMat, iX, iY, iXDir, iYDir);
}

global func FeedFuel(pFuel, pConsumer, iDemand) { //brennstoffobjekt, ofen, wie viel gleich "eingesetzt" wird und daher nicht in den "wärmespeicher" geht
  if(!pConsumer)
   pConsumer = this;
  var iValue = pFuel->~FurnaceFuelValue();
  if(!iValue)
    return false;
  var vBurn = pFuel->~BurnUp(pConsumer); 
  if(vBurn == -1) //have BurnUp return -1 to reject consumption
    return false; 
  if(!vBurn) //wenn die Verbrennung nicht überladen ist (wie es das Ölfass tut), das Brennmaterial einfach entfernen
    RemoveObject(pFuel);
  pConsumer->~Fuelled(Max(0,iValue-iDemand)); //"Restwärme" spenden (wenn der Ofen ein Wärmespeichersystem hat)
  return iValue;
}

global func GetFuelEfficiency(vFuel) //Brennwert durch Clunker...
{
  var iValue;
  if(GetType(vFuel) == C4V_C4Object)
    iValue = GetValue(vFuel);
  if(GetType(vFuel) == C4V_C4ID)
    iValue = GetValue(0,vFuel);
  if(iValue)
    return 100 * vFuel->~FurnaceFuelValue() / iValue;
}

global func GetAllFuels()
{
  var aFuels = [];
  var aEffs = [];
  var idObj;
  for(var i=0; idObj = GetDefinition(i); i++)
  {
    var iEfficiency = GetFuelEfficiency(idObj);
    if(iEfficiency)
	  for(var j = 0; j <= GetLength(aEffs); j++)
	    if(aEffs[j] <= iEfficiency || GetLength(aEffs) == j)
		{
		  InsertArrayItem(idObj, j, aFuels);
		  InsertArrayItem(iEfficiency, j, aEffs);
		  break;
		}
  }
  return aFuels;
}

global func CheckContainedFuel(pContainer)
{
  if(!pContainer)
    pContainer = this;
  var iContainedValue = pContainer ->~ AccessStoredFuel();
  var i = 0, pPotentialFuel;
  while(pPotentialFuel = Contents(i++,pContainer))
  {
    iContainedValue += pPotentialFuel->~FurnaceFuelValue();
  }
  return iContainedValue;
}

global func ConsumeFuel(iAmount, pPlant, pWorker, fSilent) //von ProductionOrder o. ä. aufzurufen
{
  if(CheckContainedFuel(pPlant) < iAmount)
    if(pWorker)
	 return DemandFuel(iAmount, 0, pWorker, pPlant);
	else
	{
	  if(!fSilent)
	    Message("$MsgFuelNotAvailable$", pPlant);
	  return false;
	}
  iAmount -= pPlant ->~ AccessStoredFuel(iAmount);
  var i = 0, iEnergy, pPotentialFuel;
  while(pPotentialFuel = Contents(i++, pPlant))
  {
    iEnergy = FeedFuel(pPotentialFuel, pPlant, iAmount); //wenn die Energie iAmount überschreitet, wird der Rest per Fuelled eingespeist und gespeichert, wenn der Ofen das kann...
	if(!iEnergy)
	  pPlant ->~ EjectNonFuel(pPotentialFuel); //ggf. rausschmeißen, was nicht verbrennt
	iAmount -= iEnergy;
	if(iAmount <= 0)
	  return true;
  }
  return false;
}

global func DemandFuel(iAmount, iFuelIndex, pWorker, pPlant)
{
  if(!pPlant)
    pPlant = this;
  var aFuelArray = GetAllFuels();
  if(iFuelIndex >= GetLength(aFuelArray))
  {
    Message("$MsgFuelNotAvailable$", pPlant);
	return false;
  }
  var idFuel = aFuelArray[iFuelIndex];
  //Message("Feed me %d/%d units of {{%i}}, %v", pPlant, Max(iAmount-CheckContainedFuel(pPlant),0), iAmount, idFuel, pWorker);
  if(CheckContainedFuel(pPlant) >= iAmount) return true; //genug Brennstoff da? aufhören
  // Der Arbeiter soll das Material beschaffen
  AddCommand(pWorker,"Call", pPlant, iAmount,iFuelIndex, 0, 0, "CmdAcquireFuel", 0, 3);
  //beachte: addcommand hat noch einen intervall-parameter. daher die null vor dem string
  AddCommand(pWorker, "Put", pPlant, 0,0, 0, 0, idFuel);
  AddCommand(pWorker, "Acquire", 0,0, 0, pPlant, 0, idFuel);
  /*var i = 0, szCmd = "";
  while(GetCommand(pWorker,,i++))
   szCmd = Concat(szCmd, GetCommand(pWorker,,i));
  Log(szCmd);*/
  return 1;
}


global func CmdAcquireFuel(pWorker, iAmount, iIndex)
{
  //Message("%v demands more fuel (%d)", pWorker, pWorker, iAmount);
  DemandFuel(iAmount, iIndex, pWorker, this);   //mehr holen
}

global func CmdAcquireFuelFailed(pWorker, iAmount, iIndex)
{
  //Message("%v failed and demands fuel (%d)", pWorker, pWorker, iAmount);
  DemandFuel(iAmount, iIndex+1, pWorker, this);  //den nächstschlechteren Brennstoff versuchen
}

//lokale Funktionen

func Fuelled(iAmount)
{
  iStoredFuel += iAmount;
}

func AccessStoredFuel(iAmount)
{
 if(!iAmount)
   return iStoredFuel;
 var iReturn = BoundBy(iAmount, 0, iStoredFuel);
 iStoredFuel -= iReturn;
 return iReturn;
}
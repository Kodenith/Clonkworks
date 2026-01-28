/*-- Baubeschleuniger --*/

#strict

local Acceleration; // Beschleunigungsfaktor

protected func Initialize()
{
  SetAction("Initialize");
  SetPosition(50,70);
  return(1);
}
  
/* Verzögerte Initialisierung */  
  
private func Initialized()
{
  // Beschleunigungsfaktor ermitteln
  Acceleration = 1;
  //Log("Build acceleration: %d", Acceleration);
  // Alle übrigen Steuerungsobjekte dieser Art entfernen
  //var pOther;
  //while (pOther = FindObject(GetID(this())))
  //  RemoveObject(pOther);
  // Steuerungsaktion
  SetAction("BuildAccelerator");
}
  
/* Beschleunigen */  
  
private func Accelerating()
{
  // Bauende Clonks suchen
  var pClonk;
  while (pClonk = FindObject(0, 0,0,0,0, OCF_Alive(), "Build",0, NoContainer(), pClonk))
    if (pClonk->~IsClonk())
    {
      // Gebäude aufstocken
      pClonk->GetActionTarget()->SetCon(Min(pClonk->GetActionTarget()->GetCon() + Acceleration, 100));
      //Log("Accelerating: %s + %d", pClonk->GetActionTarget()->GetName(), Acceleration);
    }
}

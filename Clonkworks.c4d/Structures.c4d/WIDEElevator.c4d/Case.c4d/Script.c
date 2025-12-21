/*-- Fahrstuhlkorb --*/

#strict
#include ELEC
protected func Timer() 
{
  timer = (++timer) % 30;
  // Lore fassen
  GrabObjects();
  // Clonks umgreifen lassen
  ChangeClonkGrabs();
  // Feststecken -> Schachtbohrung
  if (Stuck() && GetComDir() != COMD_Stop())
    if (!NoEnergy())
      DigFreeRect(GetX() - 24, GetY() - 13, 49, 26);
  if (!timer) Activity();
}

private func FitsInElevator(pObject)
{
  // Alle (ungedrehten) Vertices des Objekts prüfen
  var vertexCount = GetDefCoreVal("Vertices", "DefCore", pObject->GetID());
  for (var i = 0; i < vertexCount; i++) 
  {
    var vertexX = GetDefCoreVal("VertexX", "DefCore", pObject->GetID(), i);
    var vertexY = GetDefCoreVal("VertexY", "DefCore", pObject->GetID(), i);
    // Passt nicht
    if (!Inside(vertexX, -23, 23) || !Inside(vertexY, -30, 15)) return(0);
  }
  // Passt
  return(1);  
}

private func Passenger()
{
  // Clonk finden
  var clnk = FindObject(0, -24,-13,24+16,20, OCF_CrewMember(),0,0,NoContainer());
  if(!clnk) return (0);
  // steht wirklich auf dem Fahrstuhl?
  if(GetProcedure(clnk) ne "WALK") return (0);
  return (clnk);
}

private func FindWaitingClonk()
{
  var clnk, best, proc;
  while(clnk = FindObject(0, -80, RangeTop - GetY(), 120, 6000, OCF_CrewMember, 0, 0, NoContainer(), clnk) )
  {
    proc = GetProcedure(clnk);
    if(GetComDir(clnk) == COMD_Stop || (proc eq "SWIM" && Inside(GetXDir(clnk), -5, 5)) )
    {
      if(proc eq "WALK" || proc eq "PUSH" || proc eq "SCALE" || proc eq "HANGLE" || proc eq "SWIM")
        if(!Hostile(GetController(clnk), GetOwner()) )
        {
          // Nicht erreichbar?
          if(GetY(clnk) < GetY() - 7) if(!PathFree(GetX(), GetY(), GetX(), GetY(clnk)) ) continue;
          if(GetY(clnk) > GetY() + 7) if(!PathFree(GetX(), GetY() + 16, GetX(), GetY(clnk)) ) continue;
          // Priorität prüfen (Cursor hat Priorität, sonst nächster Clonk)
          if(!best)
            best = clnk;
          else if(GetCursor(GetController(clnk)) == clnk)
            best = clnk;
          else if(GetCursor(GetController(best)) != best)
            if(ObjectDistance(clnk) < ObjectDistance(best))
              best = clnk;
        }
    }
  }
  return best;
}

private func ChangeClonkGrabs()
{
  var clonk;
  var clonk_ocf = OCF_Living() | OCF_NotContained();
  // Schiebende Clonks suchen
  while (clonk = FindObject(0, -15, -10, 30, 20, clonk_ocf, "Push", 0, 0, clonk)) 
  {
    if (GetComDir(clonk) != COMD_Stop() && GetComDir(clonk) != COMD_Up()) continue;
    if (!Inside(GetXDir(clonk), -2, +2)) continue;
    if (GetCommand(clonk) && GetCommand(clonk) ne "None") continue;
    if (GetMenu(clonk)) continue;
      
    // Geschobenes Objekt prüfen
    var target = GetActionTarget(0, clonk);
    if (!target) continue;
    if (target->~IsElevator() )
    {
      if(target == this())
      {
        // In den Fahrstuhlkorb laufen, wenn der Clonk am Rand steht
        if(!Inside(GetX(clonk) - GetX(), -16, +16))
        {
          SetCommand(clonk, "Grab", this());
          AddCommand(clonk, "MoveTo", 0, BoundBy(clonk->GetX(), GetX()-5, GetX()+5), GetY());
          AddCommand(clonk, "UnGrab");
        }
      }

      continue;
    }

    if (!Inside(GetX(target), GetX() - 16, GetX() + 16)) continue;
    if (ObjectDistance(this(), target) > 15) continue;
    if (!Inside(GetXDir(target), -2, +2)) continue;
    if (!PathFree(GetX(this()), GetY(this()), GetX(target), GetY(target))) continue;
    if (!FitsInElevator(target)) continue;
      
    // Geschobenes Objekt zentrieren
    GrabAdjustPosition(target);
    // Der Clonk soll nun uns anfassen
    if (!Inside(GetX(clonk) - GetX(), -5, +5)) 
    {
      //SetCommand(clonk, "UnGrab"); unnötig, wenn wir unten SetCommand benutzen...
      //AppendCommand(clonk, "MoveTo", 0, GetX()); wtf sollte denn dieses MoveTo-Kommando zur oberen Bildschirmkante...?
      SetCommand(clonk, "Grab", this());

      // Clonk soll in die Mitte laufen, wenn das Target gerade erst
      // reingeschoben wurde, sonst steht der Clonk so blöd am Rand des
      // Fahrstuhls und lässt beim Runterfahren den Korb los.
      if(!Inside(GetX(clonk) - GetX(), -8, +8))
      {
        AddCommand(clonk, "MoveTo", 0, BoundBy(clonk->GetX(), GetX()-5, GetX()+5), GetY());
        AddCommand(clonk, "UnGrab");
      }
    } 
    else 
    {
      SetActionTargets(this(), 0, clonk);
    }
  }
}

private func Drilling()
{
  // Steuermann verloren? Suche schiebenden Clonk selbst wenn er ein Fahrzeug im Fahrstuhlkorb schiebt (->Lore)
  var elev = Elevator();
  if (!elev) return Halt();
  if (!FindObject2(Find_InRect(-28,-28,55,55), Find_Action("Push")))
    return Halt();
  // Energieverbrauch
  if (ObjectCount(ENRG))
    if (EnergyCheck(10000, elev))
      DoEnergy(-10000, elev, true);
    else
      Halt();
  return(1);
}
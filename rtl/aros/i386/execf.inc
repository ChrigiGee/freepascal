{
    This file is part of the Free Pascal run time library.
    Copyright (c) 2006 Karoly Balogh

    exec functions (V40) for Amiga/PowerPC

    See the file COPYING.FPC, included in this distribution,
    for details about the copyright.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

 **********************************************************************}

var
  LocalExecBase   : Pointer; external name '_ExecBase';


function execAllocMem(byteSize: Cardinal; requirements: Cardinal): Pointer; syscall LocalExecBase 33;
procedure execFreeMem(memoryBlock: Pointer; byteSize: Cardinal); syscall LocalExecBase 35;
function FindTask(name: PChar): PTask; syscall LocalExecBase 49;
function SetSignal(newSignals: Cardinal; signalSet : Cardinal): Cardinal; syscall LocalExecBase 51;
function AllocSignal(signalNum: LongInt): ShortInt; syscall LocalExecBase 55;
procedure FreeSignal(signalNum: LongInt); syscall LocalExecBase 56;
procedure AddPort(port: PMsgPort); syscall LocalExecBase 59;
procedure RemPort(port: PMsgPort); syscall LocalExecBase 60;
function GetMsg(port: PMsgPort): PMessage; syscall LocalExecBase 62;
procedure ReplyMsg(message : pMessage); syscall LocalExecBase 63;
function WaitPort(port: PMsgPort): PMessage; syscall LocalExecBase 64;
procedure CloseLibrary(libHandle: PLibrary); syscall LocalExecBase 69;
function OpenDevice(devName: PChar; numunit: Cardinal; ioRequest: pIORequest; flags: Cardinal): ShortInt; syscall LocalExecBase 74;
procedure CloseDevice(ioRequest: PIORequest); syscall LocalExecBase 75;
function DoIO(ioRequest: PIORequest): ShortInt; syscall LocalExecBase 76;
function OpenLibrary(libname: PChar; libver : Cardinal): Pointer; syscall LocalExecBase 92;
function CreatePool(requirements: Cardinal; puddleSize: Cardinal; threshSize: Cardinal): Pointer; syscall LocalExecBase 116;
procedure DeletePool(poolHeader: Pointer); syscall LocalExecBase 117;
function AllocPooled(poolHeader: Pointer; memSize: Cardinal): Pointer; syscall LocalExecBase 118;
function FreePooled(poolHeader: Pointer; memory: Pointer; memSize: Cardinal): Pointer; syscall LocalExecBase 119;

//function RawDoFmt(const formatString : pCHAR;const dataStream : POINTER; putChProc : tPROCEDURE; putChData : POINTER): pointer;
function RawDoFmt(const formatString : pCHAR;const dataStream : POINTER; putChProc : POINTER; putChData : POINTER): pointer; syscall LocalExecBase 87;

function GetLibAdress(Base: Pointer; Offset: LongInt): Pointer; inline;
begin
  if Base = nil then
  begin
    RawDoFmt('FPC_FILE_DEBUG: Error! Illegal library access with not opened library: %d !'+#10,@Offset,pointer(1),nil);
    Debugln('Illegal library access with not opened library');
    Halt(1);
  end;  
  GetLibAdress := Pointer((Base -(Offset * SizeOf(Pointer)))^);
end;

procedure Debug(s: string);
type
  TkPrintf = function(Msg: PChar): Integer; cdecl;
var
  kPrintf: TkPrintf;
  d: string;
  i: Integer;
begin
  if Assigned(AOS_ExecBase) then
  begin
    if Assigned(PExecBase(AOS_ExecBase)^.DebugArosBase) then
    begin
      kPrintf := TKPrintF(PExecBase(AOS_ExecBase)^.DebugArosBase^.kprintfPtr);
      if Assigned(kPrintf) then
      begin
        d := s + #0;
        i := kPrintf(@d[1]);
      end;
    end;
  end;
end;

procedure Debugln(s: string);
begin
  Debug(s + #10);
end;
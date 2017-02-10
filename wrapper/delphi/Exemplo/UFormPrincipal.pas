unit UFormPrincipal;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, Balanca, StdCtrls;

type
  TForm1 = class(TForm)
    Balanca1: TBalanca;
    Label1: TLabel;
    LabelPeso: TLabel;
    ListBox1: TListBox;
    Label3: TLabel;
    LabelStatus: TLabel;
    Button1: TButton;
    Label5: TLabel;
    Edit1: TEdit;
    Label2: TLabel;
    LabelTotal: TLabel;
    procedure Balanca1Conectado(Sender: TObject);
    procedure Balanca1Desconectado(Sender: TObject);
    procedure Balanca1PesoRecebido(Sender: TObject; Gramas: Integer);
    procedure FormCreate(Sender: TObject);
    procedure Button1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation

{$R *.dfm}

procedure TForm1.Balanca1Conectado(Sender: TObject);
begin
  Button1.Enabled := True;
  LabelStatus.Caption := 'Conectado';
  LabelStatus.Font.Color := clGreen;
  ListBox1.AddItem('Conectado', nil);
end;

procedure TForm1.Balanca1Desconectado(Sender: TObject);
begin
  // a conex�o com balan�a foi encerrada, o reestabelecimento � autom�tico
  // isso pode ter sido provocado por cabo da balan�a mal encaixado ou demora
  // no envio do peso
  Button1.Enabled := False;
  LabelStatus.Caption := 'Desconectado';
  LabelStatus.Font.Color := clRed;
  ListBox1.AddItem('Desconectado', nil);
end;

procedure TForm1.Balanca1PesoRecebido(Sender: TObject; Gramas: Integer);
var
  str: string;
begin
  // converte de grama para Kilo
  if Gramas >= 1000 then
    str := FormatFloat('0.000', Gramas / 1000) + ' kg'
  else
    str := IntToStr(Gramas) + ' g';
  LabelPeso.Caption := str;
  ListBox1.AddItem('Peso recebido ' + str, nil);
  // calcula o total
  LabelTotal.Caption := 'R$ '  + FormatCurr('0.00', StrToFloat(Edit1.Text) * Gramas / 1000);
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
  // � melhor ativar somente depois que o componente da balan�a
  // tenha sido criado
  Balanca1.Ativo := True;
end;

procedure TForm1.Button1Click(Sender: TObject);
begin
  // envia o pre�o para a balan�a e solicita o peso
  Balanca1.Preco := StrToFloat(Edit1.Text);
  // Solicita o peso, sem alterar o pre�o (Mesmo que Balanca1.Preco := 0;)
  //Balanca1.SolicitaPeso;
end;

end.

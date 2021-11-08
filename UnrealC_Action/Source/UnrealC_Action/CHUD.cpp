#include "CHUD.h"
#include "Global.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"

ACHUD::ACHUD()
{
	CHelpers::GetAsset<UTexture2D>(&Texture, "Texture2D'/Game/Textures/T_Crosshair.T_Crosshair'");
}

void ACHUD::DrawHUD()
{
	Super::DrawHUD();
	CheckFalse(bDraw);
	CheckNull(Texture);

	//ȭ���� �߽� ���ϱ� Canvas : 2Dȭ�� 1:1
	FVector2D center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	FVector2D size(Texture->GetSurfaceWidth() * 0.5f, Texture->GetSurfaceHeight() * 0.5f);
	FVector2D position = center - size;//�� �߰��� ��ġ�ϰ�
	
	FCanvasTileItem item(position, Texture->Resource, FLinearColor::White);//White ���� ���ϱ�
	item.BlendMode = SE_BLEND_Translucent;//���� ���ϸ� ���ڸ�

	Canvas->DrawItem(item);
}
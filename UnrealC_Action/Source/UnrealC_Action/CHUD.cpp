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

	//화면의 중심 구하기 Canvas : 2D화면 1:1
	FVector2D center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	FVector2D size(Texture->GetSurfaceWidth() * 0.5f, Texture->GetSurfaceHeight() * 0.5f);
	FVector2D position = center - size;//딱 중간에 위치하게
	
	FCanvasTileItem item(position, Texture->Resource, FLinearColor::White);//White 원본 곱하기
	item.BlendMode = SE_BLEND_Translucent;//일정 이하면 다자름

	Canvas->DrawItem(item);
}
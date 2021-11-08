#include "CArrow.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Characters/CPlayer.h"

ACArrow::ACArrow()
{
	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere");
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle", Sphere);
	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");

	InitialLifeSpan = 3.0f;

	Projectile->InitialSpeed = 2000.0f;
	Projectile->MaxSpeed = 4000.0;
	Projectile->ProjectileGravityScale = 0.0f;
}

void ACArrow::BeginPlay()
{
	Super::BeginPlay();

	TArray<USphereComponent*>components;
	GetComponents<USphereComponent>(components);

	for (UShapeComponent* component : components)
		component->OnComponentBeginOverlap.AddDynamic(this, &ACArrow::OnComponentBeginOverlap);
}

void ACArrow::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPlayer* player = Cast<ACPlayer>(OtherActor);
	if (player != NULL)
		return;


	if (!!Explosion)
	{
		FTransform transform = ExplosionTransform;
		transform.AddToTranslation(SweepResult.Location);
		transform.SetRotation(FQuat(SweepResult.ImpactNormal.Rotation()));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, transform);
	}

	if (OnArrowBeginOverlap.IsBound())
		OnArrowBeginOverlap.Broadcast(SweepResult);

	Destroy();
}

#include "CThrow.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "Characters/CPlayer.h"
#include "Animation/SkeletalMeshActor.h"

ACThrow::ACThrow()
{
	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere");
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle", Sphere);
	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");

	InitialLifeSpan = 5.0f;

	Projectile->InitialSpeed = 2000.0f;
	Projectile->MaxSpeed = 4000.0;
	Projectile->ProjectileGravityScale = 0.0f;

	//Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Particle->Deactivate();

	
}

void ACThrow::BeginPlay()
{
	Super::BeginPlay();

	TArray<USphereComponent*>components;
	GetComponents<USphereComponent>(components);

	for (UShapeComponent* component : components)
		component->OnComponentBeginOverlap.AddDynamic(this, &ACThrow::OnComponentBeginOverlap);


	//if (bChargeSkill == true)
	//{
	//	Particle->ActivateSystem(true);
	//}
}


void ACThrow::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OtherActor == GetOwner());

	ACPlayer* player = Cast<ACPlayer>(OtherActor);



	if (!!Explosion)
	{	
		FTransform transform = ExplosionTransform;
		transform.AddToTranslation(bFromSweep ? SweepResult.Location : GetActorLocation());
		transform.SetRotation(FQuat(SweepResult.ImpactNormal.Rotation()));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, transform);
	}

	else if (!!Explosion_Niagara)
	{
		FTransform transform = ExplosionTransform;
		transform.AddToTranslation(bFromSweep ? SweepResult.Location : GetActorLocation());
		transform.SetRotation(FQuat(SweepResult.ImpactNormal.Rotation()));	
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Explosion_Niagara, transform.GetLocation(), SweepResult.ImpactNormal.Rotation());

		/*if (bChargeSkill == true)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Explosion_Niagara, transform.GetLocation(), SweepResult.ImpactNormal.Rotation(), FVector(6));
		}
		else
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Explosion_Niagara, transform.GetLocation(), SweepResult.ImpactNormal.Rotation());
		}*/
	}

	if (OnThrowBeginOverlap.IsBound())
		OnThrowBeginOverlap.Broadcast(SweepResult);

	//¸Â¾ÑÀ»‹š ¿òÁ÷ÀÓ ¸ØÃã, Ãæµ¹Ã¼ ²ô±â
	Projectile->StopMovementImmediately();
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Destroy();
}

//TODO : Charge½ºÅ³ Â÷º°È­
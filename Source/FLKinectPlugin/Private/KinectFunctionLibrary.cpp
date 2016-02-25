#include "FLKinectPluginPrivatePCH.h"

FVector UKinectPluginFunctionLibrary::GetJointPosition(FFLKinect_Body const& body, EFLKinect_JointType type)
{
	return body.positions[static_cast<uint8>(type)];
}

FRotator UKinectPluginFunctionLibrary::GetJointOrientation(FFLKinect_Body const& body, EFLKinect_JointType type)
{
	return body.orientations[static_cast<uint8>(type)];
}

EFLKinect_TrackingState UKinectPluginFunctionLibrary::GetTrackingState(UPARAM(ref) FFLKinect_Body const& body, EFLKinect_JointType type)
{
	return body.trackingStates[static_cast<uint8>(type)];
}


void UKinectPluginFunctionLibrary::GetKinectBody(uint8 number, FFLKinect_Body& out)
{
	out = *FKinectPluginCore::GetInstance()->GetBody(number);
}

void UKinectPluginFunctionLibrary::GetKinectTrackedBodies(TArray<bool>& out)
{
	out.Empty(BODY_COUNT);
	out.Append(FKinectPluginCore::GetInstance()->TrackFlags, BODY_COUNT);
}


bool UKinectPluginFunctionLibrary::IsKinectBodyTracked(uint8 number)
{
	check(number < BODY_COUNT);
	return FKinectPluginCore::GetInstance()->TrackFlags[number];
}

bool UKinectPluginFunctionLibrary::GetFirstActiveKinectBody(FFLKinect_Body& out)
{
	auto* instance = FKinectPluginCore::GetInstance();
	for (int32 i = 0; i < BODY_COUNT; ++i)
	{
		if (instance->TrackFlags[i])
		{
			out = *instance->GetBody(i);
			return true;
		}
	}
	return false;
}

void UKinectPluginFunctionLibrary::GetAllPositions(uint8 num, TArray<FVector>& out)
{
	out.Empty(FFLKinect_Body::nJoints);
	out.Append(FKinectPluginCore::GetInstance()->GetBody(num)->positions, FFLKinect_Body::nJoints);
}

void UKinectPluginFunctionLibrary::DrawBody(const AActor* worldActor, FFLKinect_Body& body, FTransform transform, float PointSize, float LineLength)
{
	for (int32 i = 0; i < body.nJoints; ++i)
	{
		auto position = transform.TransformFVector4(body.positions[i]);
		auto forward = body.orientations[i].RotateVector(FVector::ForwardVector * LineLength);
		//TODO (OS): Check the math here
		auto endEffectorPosition = position + transform.TransformFVector4(forward);
		DrawDebugPoint(worldActor->GetWorld(), position, PointSize, FColor::Red, false, 0.03);
		DrawDebugLine(worldActor->GetWorld(), position, endEffectorPosition, FColor::Green, false, 0.03, 0, PointSize * 0.3);
	}
}
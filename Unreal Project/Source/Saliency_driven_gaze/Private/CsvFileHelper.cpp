// Fill out your copyright notice in the Description page of Project Settings.


#include "CsvFileHelper.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"


bool UCsvFileHelper::SaveFixationTableToCsvFile(FString SaveDirectory, FString FileName, TArray<FFixationData> FixationTable, bool AllowOverWriting)
{
	// Set complete file path
	SaveDirectory += "\\";
	SaveDirectory += FileName;

	if (!AllowOverWriting) {
		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*SaveDirectory))
		{
			return false;
		}
	}


	FString FinalString = "";

	// Write header
	{
		FinalString += "Name";
		FinalString += ",Position";
		FinalString += ",Duration";
		FinalString += ",TimeStamp";
		FinalString += ",ObjectName";
		FinalString += ",ObjectPosition";
		FinalString += ",ObjectRotation";
		FinalString += ",ObjectScale";
		FinalString += ",HitPosition";
		FinalString += ",HitNormal";
		FinalString += ",EyeDirection";
		FinalString += ",HeadPosition";
		FinalString += ",HeadRotation";
		FinalString += ",EyeLeftBlink";
		FinalString += ",EyeRightBlink";
		FinalString += ",GazeLeftCoord";
		FinalString += ",GazeRightCoord";
		FinalString += ",GazeCombineCoord";
		FinalString += ",Tracked";
		FinalString += LINE_TERMINATOR;
	}

	// Format table rows as strings
	for (int i = 0; i < FixationTable.Num(); i++)
	{
		FinalString += FIntToFString(i);
		FinalString += "," + FVectorToFString(FixationTable[i].Position);
		FinalString += "," + FFloatToFString(FixationTable[i].Duration);
		FinalString += "," + FFloatToFString(FixationTable[i].TimeStamp);
		FinalString += "," + FixationTable[i].ObjectName;
		FinalString += "," + FVectorToFString(FixationTable[i].ObjectPosition);
		FinalString += "," + FRotatorToFString(FixationTable[i].ObjectRotation);
		FinalString += "," + FVectorToFString(FixationTable[i].ObjectScale);
		FinalString += "," + FVectorToFString(FixationTable[i].HitPosition);
		FinalString += "," + FVectorToFString(FixationTable[i].HitNormal);
		FinalString += "," + FVectorToFString(FixationTable[i].EyeDirection);
		FinalString += "," + FVectorToFString(FixationTable[i].HeadPosition);
		FinalString += "," + FRotatorToFString(FixationTable[i].HeadRotation);
		FinalString += "," + FFloatToFString(FixationTable[i].EyeLeftBlink);
		FinalString += "," + FFloatToFString(FixationTable[i].EyeRightBlink);
		FinalString += "," + FVector2DToFString(FixationTable[i].GazeLeftCoord);
		FinalString += "," + FVector2DToFString(FixationTable[i].GazeRightCoord);
		FinalString += "," + FVector2DToFString(FixationTable[i].GazeCombineCoord);
		FinalString += "," + FBoolToFString(FixationTable[i].Tracked);
		FinalString += LINE_TERMINATOR;
	}

	return FFileHelper::SaveStringToFile(FinalString, *SaveDirectory);
}

bool UCsvFileHelper::SaveExperimentTableToCsvFile(FString SaveDirectory, FString FileName, TArray<FString> UserIDs, TArray<FExperimentData> ExperimentTable, bool AllowOverWriting)
{
	// Set complete file path
	SaveDirectory += "\\";
	SaveDirectory += FileName;

	if (!AllowOverWriting) {
		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*SaveDirectory))
		{
			return false;
		}
	}


	FString FinalString = "";

	// Write header
	{
		FinalString += "Name";
		FinalString += ",Done";
		FinalString += ",Level_1_void";
		FinalString += ",Level_1_populated";
		FinalString += ",Level_1_void_event";
		FinalString += ",Level_1_populated_event";
		FinalString += ",Level_2_void";
		FinalString += ",Level_2_populated";
		FinalString += ",Level_2_void_event";
		FinalString += ",Level_2_populated_event";
		FinalString += ",Level_3_void";
		FinalString += ",Level_3_populated";
		FinalString += ",Level_3_void_event";
		FinalString += ",Level_3_populated_event";
		FinalString += LINE_TERMINATOR;
	}

	// Format table rows as strings
	for (int i = 0; i < ExperimentTable.Num(); i++)
	{
		FinalString += UserIDs[i];
		FinalString += "," + FIntToFString(ExperimentTable[i].Done);
		FinalString += "," + FIntToFString(ExperimentTable[i].Level_1_void);
		FinalString += "," + FIntToFString(ExperimentTable[i].Level_1_populated);
		FinalString += "," + FIntToFString(ExperimentTable[i].Level_1_void_event);
		FinalString += "," + FIntToFString(ExperimentTable[i].Level_1_populated_event);
		FinalString += "," + FIntToFString(ExperimentTable[i].Level_2_void);
		FinalString += "," + FIntToFString(ExperimentTable[i].Level_2_populated);
		FinalString += "," + FIntToFString(ExperimentTable[i].Level_2_void_event);
		FinalString += "," + FIntToFString(ExperimentTable[i].Level_2_populated_event);
		FinalString += "," + FIntToFString(ExperimentTable[i].Level_3_void);
		FinalString += "," + FIntToFString(ExperimentTable[i].Level_3_populated);
		FinalString += "," + FIntToFString(ExperimentTable[i].Level_3_void_event);
		FinalString += "," + FIntToFString(ExperimentTable[i].Level_3_populated_event);
		FinalString += LINE_TERMINATOR;
	}

	return FFileHelper::SaveStringToFile(FinalString, *SaveDirectory);
}

bool UCsvFileHelper::SaveScenePointsToPlyFile(FString SaveDirectory, FString FileName, TArray<FVector> Points, bool AllowOverWriting)
{
	// Set complete file path
	SaveDirectory += "\\";
	SaveDirectory += FileName;

	if (!AllowOverWriting) {
		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*SaveDirectory))
		{
			return false;
		}
	}


	FString FinalString = "";

	// Write header
	{
		FinalString += "ply\n";
		FinalString += "format ascii 1.0\n";
		FinalString += "element vertex " + FIntToFString(Points.Num()) + "\n";
		FinalString += "property float x\n";
		FinalString += "property float y\n";
		FinalString += "property float z\n";
		//FinalString += "property float nx\n";
		//FinalString += "property float ny\n";
		//FinalString += "property float nz\n";
		//FinalString += "property float r\n";
		//FinalString += "property float g\n";
		//FinalString += "property float b\n";
		FinalString += "property float s\n";
		FinalString += "end_header\n";
	}

	// oints as strings
	for (int i = 0; i < Points.Num(); i++)
	{
		// Position
		FinalString += FString::SanitizeFloat(Points[i].X);
		FinalString += " ";
		FinalString += FString::SanitizeFloat(Points[i].Y);
		FinalString += " ";
		FinalString += FString::SanitizeFloat(Points[i].Z);
		FinalString += " ";
		
		// Normal
		//...
		
		// Color
		//...
		
		// Saliency
		FinalString += "0.0";
		FinalString += "\n";
	}

	return FFileHelper::SaveStringToFile(FinalString, *SaveDirectory);
}

int UCsvFileHelper::LoadPointCloudPlyFile(FString LoadDirectory, FString FileName, UPARAM(ref) TArray<FString>& PlyJson)
{
	int nbPoints = 0;

	// Set complete file path
	LoadDirectory += "\\";
	LoadDirectory += FileName;
	if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*LoadDirectory))
	{
		if (FFileHelper::LoadFileToStringArray(PlyJson, *LoadDirectory))
		{
			FString line = "";
			FString value = "";
			bool in_header = true;

			// Skip the header
			while (in_header)
			{
				line = PlyJson[0];

				// End header
				if (line.Equals("end_header")) {
					in_header = false;
				}

				// Extract 1st word
				line.Split(" ", &value, &line);

				// Get number of points
				if (value.Equals("element")) {
					line.Split(" ", &value, &line);
					nbPoints = FCString::Atoi(*line);
				}

				PlyJson.RemoveAt(0);
			}	
		}
	}
	
	return nbPoints;
}

//bool UCsvFileHelper::LoadPointsJson(UPARAM(ref) TArray<FString>& PlyJson, UPARAM(ref) TArray<FVector>& pointPositions, UPARAM(ref) TArray<FColor>& pointColors, int nbPointsMax, int modulo)
//{
//	FString line = "";
//	FString value = "";
//
//	for (int i = 0; i < nbPointsMax; i++) 
//	{
//		if (PlyJson.Num() <= modulo) {
//			return false;
//		}
//
//		for (int j = 0; j < modulo; j++) 
//		{
//			PlyJson.HeapPop(line);
//		}
//
//		PlyJson.HeapPop(line);
//		
//		// Extract 1st word
//		line.Split(" ", &value, &line);
//		float x = FCString::Atof(*value);
//		line.Split(" ", &value, &line);
//		float y = FCString::Atof(*value);
//		line.Split(" ", &value, &line);
//		float z = FCString::Atof(*value);
//		FVector pos(x, y, z);
//
//		// Read saliency value
//		float s = FCString::Atof(*line);
//		//FColor saliency(s, s, s);
//		FColor saliency(1.0f, 1.0f, 1.0f, 1.0f);
//
//		// Add to arrays
//		pointPositions.Add(pos);
//		pointColors.Add(saliency);
//	}
//
//	return true;
//}

TArray<FLinearColor> UCsvFileHelper::LoadPointsJson(UPARAM(ref) TArray<FString>& PlyJson, int nbPoints)
{
	TArray<FLinearColor> pointsColor;

	FString line = "";
	FString value = "";

	for (int i = 0; i < nbPoints; i++)
	{
		if (PlyJson.Num() <= 0) {
			break;
		}

		line = PlyJson[0];

		// Extract 1st word
		line.Split(" ", &value, &line);
		float x = FCString::Atof(*value);
		line.Split(" ", &value, &line);
		float y = FCString::Atof(*value);
		line.Split(" ", &value, &line);
		float z = FCString::Atof(*value);

		// Read saliency value
		float s = FCString::Atof(*line);
		
		// Add to arrays
		FLinearColor saliency(x, y, z, s);
		pointsColor.Add(saliency);

		PlyJson.RemoveAt(0);
	}

	return pointsColor;
}

////////////////////////////////////////////////
//					 UTILS					  //
////////////////////////////////////////////////

FString UCsvFileHelper::FIntToFString(int value)
{
	return FString::FromInt(value);
}

FString UCsvFileHelper::FBoolToFString(bool value)
{
	FString result = "";
	result += "\"";
	result += value ? "True" : "False";
	result += "\"";
	return result;
}

FString UCsvFileHelper::FFloatToFString(float value)
{
	return "\"" + FString::SanitizeFloat(value) + "\"";
}

FString UCsvFileHelper::FVectorToFString(FVector vector)
{
	FString result = "";
	result += "\"(";
	result += "X=" + FString::SanitizeFloat(vector.X);
	result += ",";
	result += "Y=" + FString::SanitizeFloat(vector.Y);
	result += ",";
	result += "Z=" + FString::SanitizeFloat(vector.Z);
	result += ")\"";
	return result;
}

FString UCsvFileHelper::FVector2DToFString(FVector2D vector2D)
{
	FString result = "";
	result += "\"(";
	result += "X=" + FString::SanitizeFloat(vector2D.X);
	result += ",";
	result += "Y=" + FString::SanitizeFloat(vector2D.Y);
	result += ")\"";
	return result;
}

FString UCsvFileHelper::FRotatorToFString(FRotator rotator)
{
	FString result = "";
	result += "\"(";
	result += "Pitch=" + FString::SanitizeFloat(rotator.Pitch);
	result += ",";
	result += "Yaw=" + FString::SanitizeFloat(rotator.Yaw);
	result += ",";
	result += "Roll=" + FString::SanitizeFloat(rotator.Roll);
	result += ")\"";
	return result;
}
// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "HttpModule.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "CoreMinimal.h"
#include "AlturaSDKBPLibrary.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FResponse, uint8, Status, FString, ResponseString);
UENUM(BlueprintType)
enum class SortBy : uint8
{
	address,
	name,
	bio
};
UENUM(BlueprintType)
enum class SortDir : uint8
{
	asc,
	desc
};
UENUM(BlueprintType)
enum class UserFilter : uint8
{
	none,
	address,
	name,
	bio
};
UCLASS()
class ALTURASDK_API UAlturaSDKBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()


	inline static int RequestTimeoutDuration = 60;


	static FString SortbyEnumToString(SortBy SortBy);
	static FString SortDirEnumToString(SortDir SortDir);
	static FString UserFilterEnumToString(UserFilter UserFilter);
	
	static FString AppendToUrlWithSlash(const FString& BaseUrl, const TMap<FString, FString>& Params);
	static FString AppendToUrlWithQ(const FString& BaseUrl, const TMap<FString, FString>& Params);
	static FString CreateJsonFromParams(const TMap<FString, FString>& Params);
	static FString BuildFullUrl(const FString& BaseUrl, const TMap<FString, FString>& Params,int ParsingMethods);
	
	static void MakeAHttpRequest(const FString URL, const TMap<FString, FString> Params, const TMap<FString, FString> Headers, const FString Body, const FResponse& OnComplete);

	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "VerifyAlturaGuardCode", Keywords = "VerifyAlturaGuardCode"), Category = "AlturaSDK")
	static void VerifyAlturaGuardCode(const FString Address, const FString Code, const FResponse& OnComplete);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetUser", Keywords = "GetUser"), Category = "AlturaSDK")
	static void GetUser(const FString Address, const FResponse& OnComplete);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetUsers", Keywords = "GetUsers"), Category = "AlturaSDK")
	static void GetUsers(const FString perPage, const FString page, SortBy SortBy, SortDir Sortdir, UserFilter userFilter, const FString FilterContext, const FResponse& OnComplete);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetItem", Keywords = "GetItem"), Category = "AlturaSDK")
	static void GetItem(const FString Address, const FString TokenId, const FResponse& OnComplete);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetItems", Keywords = "GetItems"), Category = "AlturaSDK")
	static void GetItems(const FString perPage, const FString page, SortBy SortBy, SortDir Sortdir,bool slim,const UserFilter userFilter,const FString FilterContext, const FResponse& OnComplete);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetItemHolders", Keywords = "GetItemHolders"), Category = "AlturaSDK")
	static void GetItemHolders(const FString Address, const FString TokenId,const FString perPage, const FString page, SortBy SortBy, SortDir Sortdir,bool includeListed, const FResponse& OnComplete);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetItemHistory", Keywords = "GetItemHistory"), Category = "AlturaSDK")
	static void GetItemHistory(FString Address, FString TokenId, FString perPage, FString page, const FResponse& OnComplete);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetCollection", Keywords = "GetCollection"), Category = "AlturaSDK")
	static void GetCollection(const FString Address, const FResponse& OnComplete);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetCollections", Keywords = "GetCollections"), Category = "AlturaSDK")
	static void GetCollections(const FString perPage, const FString page, SortBy SortBy, SortDir Sortdir, const FResponse& OnComplete);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetUserItemBalance", Keywords = "GetUserItemBalance"), Category = "AlturaSDK")
	static void GetUserItemBalance(const FString userAddress, const FString collectionAddress, const FString tokenId, const FString chainId, const FResponse& OnComplete);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetUserERC20Balance", Keywords = "GetUserERC20Balance"), Category = "AlturaSDK")
	static void GetUserERC20Balance(const FString chainId, const FString userAddress, const FString tokenAddress, const FResponse& OnComplete);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetUserNativeTokenBalance", Keywords = "GetUserNativeTokenBalance"), Category = "AlturaSDK")
	static void GetUserNativeTokenBalance(const FString chainId, const FString userAddress, const FResponse& OnComplete);
};

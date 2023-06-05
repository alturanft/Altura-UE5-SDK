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

USTRUCT(BlueprintType)
struct FAlturaJSONObject
{
	GENERATED_BODY()

		TSharedPtr<FJsonObject> JsonParsed = nullptr;

};

USTRUCT(BlueprintType)
struct FAlturaUser
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadWrite)
		FString Address;

	UPROPERTY(BlueprintReadWrite)
		FString Name;

	UPROPERTY(BlueprintReadWrite)
		FString Bio;

	UPROPERTY(BlueprintReadWrite)
		FString SocialLink;

	UPROPERTY(BlueprintReadWrite)
		FString ProfilePicUrl;
};
USTRUCT(BlueprintType)
struct FUsersRoot
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadWrite)
		TArray<FAlturaUser> Users;

	UPROPERTY(BlueprintReadWrite)
		int32 Count;
};
USTRUCT(BlueprintType)
struct FItemProperty
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadWrite, Category = "Item")
		FString Id;

	UPROPERTY(BlueprintReadWrite, Category = "Item")
		FString Name;

	UPROPERTY(BlueprintReadWrite, Category = "Item")
		FString Value;

	UPROPERTY(BlueprintReadWrite, Category = "Item")
		bool bStatic;
};
USTRUCT(BlueprintType)
struct FItem
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadWrite)
		FString CollectionAddress;

	UPROPERTY(BlueprintReadWrite)
		int32 TokenId;

	UPROPERTY(BlueprintReadWrite)
		int32 ChainId;

	UPROPERTY(BlueprintReadWrite)
		FString Name;

	UPROPERTY(BlueprintReadWrite)
		FString Description;

	UPROPERTY(BlueprintReadWrite)
		FString Image;

	UPROPERTY(BlueprintReadWrite)
		FString ImageUrl;

	UPROPERTY(BlueprintReadWrite)
		FString FileType;

	UPROPERTY(BlueprintReadWrite)
		bool bIsVideo;

	UPROPERTY(BlueprintReadWrite)
		FString CreatorAddress;

	UPROPERTY(BlueprintReadWrite)
		int32 Likes;

	UPROPERTY(BlueprintReadWrite)
		int32 Views;

	UPROPERTY(BlueprintReadWrite)
		FString MintDate;

	UPROPERTY(BlueprintReadWrite)
		int32 Royalty;

	UPROPERTY(BlueprintReadWrite)
		bool bIsNSFW;

	UPROPERTY(BlueprintReadWrite)
		int32 Supply;

	UPROPERTY(BlueprintReadWrite)
		int32 MaxSupply;

	UPROPERTY(BlueprintReadWrite)
		bool bIsStackable;

	UPROPERTY(BlueprintReadWrite)
		TArray<FItemProperty> Properties;

	UPROPERTY(BlueprintReadWrite)
		FString OtherImageVisibility;

	UPROPERTY(BlueprintReadWrite)
		bool bIsListed;

	UPROPERTY(BlueprintReadWrite)
		FString MostRecentListing;

	UPROPERTY(BlueprintReadWrite)
		float CheapestListingUSD;

	UPROPERTY(BlueprintReadWrite)
		FString CheapestListingPrice;

	UPROPERTY(BlueprintReadWrite)
		FString CheapestListingCurrency;

	UPROPERTY(BlueprintReadWrite)
		bool bIsLootBoxKey;

	UPROPERTY(BlueprintReadWrite)
		bool bIsSmartNFT;

	UPROPERTY(BlueprintReadWrite)
		int32 Holders;

	UPROPERTY(BlueprintReadWrite)
		bool bIsFeatured;

	UPROPERTY(BlueprintReadWrite)
		bool bHasUnlockableContent;

	UPROPERTY(BlueprintReadWrite)
		bool bDidLike;

	UPROPERTY(BlueprintReadWrite)
		int32 TotalListings;

	UPROPERTY(BlueprintReadWrite)
		float CheapestPriceUSD;

	UPROPERTY(BlueprintReadWrite)
		FString CreatorName;

	UPROPERTY(BlueprintReadWrite)
		FString CreatorProfilePic;

	UPROPERTY(BlueprintReadWrite)
		FString CollectionName;

	UPROPERTY(BlueprintReadWrite)
		FString URI;

	UPROPERTY(BlueprintReadWrite)
		bool bIsVerified;

	UPROPERTY(BlueprintReadWrite)
		FString Genre;

	UPROPERTY(BlueprintReadWrite)
		FString Website;

	UPROPERTY(BlueprintReadWrite)
		int32 Version;

	UPROPERTY(BlueprintReadWrite)
		FString CollectionImage;

	UPROPERTY(BlueprintReadWrite)
		FString CollectionImageUrl;

	UPROPERTY(BlueprintReadWrite)
		FString Slug;
};
USTRUCT(BlueprintType)
struct FArraySingleItem
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadWrite, Category = "Item")
		FString Name;

	UPROPERTY(BlueprintReadWrite, Category = "Item")
		TArray<FItemProperty> Properties;

	UPROPERTY(BlueprintReadWrite, Category = "Item")
		int32 TokenId;

	UPROPERTY(BlueprintReadWrite, Category = "Item")
		FString CollectionAddress;

	UPROPERTY(BlueprintReadWrite, Category = "Item")
		int32 Supply;

	UPROPERTY(BlueprintReadWrite, Category = "Item")
		int32 MaxSupply;

	UPROPERTY(BlueprintReadWrite, Category = "Item")
		FString Image;

	UPROPERTY(BlueprintReadWrite, Category = "Item")
		FString ImageUrl;

	UPROPERTY(BlueprintReadWrite, Category = "Item")
		int32 ImageIndex;

	UPROPERTY(BlueprintReadWrite, Category = "Item")
		int32 ImageCount;
};
USTRUCT(BlueprintType)
struct FHolder
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AlturaSDK")
		FString Address;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AlturaSDK")
		int32 Balance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AlturaSDK")
		FString Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AlturaSDK")
		FString ProfilePic;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AlturaSDK")
		FString ProfilePicUrl;
};

USTRUCT(BlueprintType)
struct FCollection
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadWrite)
		FString Address;

	UPROPERTY(BlueprintReadWrite)
		FString Name;

	UPROPERTY(BlueprintReadWrite)
		FString Description;

	UPROPERTY(BlueprintReadWrite)
		FString Genre;

	UPROPERTY(BlueprintReadWrite)
		FString Image;

	UPROPERTY(BlueprintReadWrite)
		FString ImageHash;

	UPROPERTY(BlueprintReadWrite)
		FString OwnerAddress;

	UPROPERTY(BlueprintReadWrite)
		FString Slug;

	UPROPERTY(BlueprintReadWrite)
		FString Uri;

	UPROPERTY(BlueprintReadWrite)
		FString Website;

	UPROPERTY(BlueprintReadWrite)
		FString MintDate;

	UPROPERTY(BlueprintReadWrite)
		int32 ChainId;

	UPROPERTY(BlueprintReadWrite)
		int32 Holders;

	UPROPERTY(BlueprintReadWrite)
		float Volume_1d;

	UPROPERTY(BlueprintReadWrite)
		float Volume_1w;

	UPROPERTY(BlueprintReadWrite)
		float Volume_30d;

	UPROPERTY(BlueprintReadWrite)
		float Volume_all;

	UPROPERTY(BlueprintReadWrite)
		FString ImageUrl;
};

USTRUCT(BlueprintType)
struct FItemHistory
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadWrite)
		FString ID;

	UPROPERTY(BlueprintReadWrite)
		FString Amount;

	UPROPERTY(BlueprintReadWrite)
		int32 BlockNumber;

	UPROPERTY(BlueprintReadWrite)
		int32 ChainId;

	UPROPERTY(BlueprintReadWrite)
		FString Event;

	UPROPERTY(BlueprintReadWrite)
		FString From;

	UPROPERTY(BlueprintReadWrite)
		FString ItemCollection;

	UPROPERTY(BlueprintReadWrite)
		FString ItemRef;

	UPROPERTY(BlueprintReadWrite)
		int32 Timestamp;

	UPROPERTY(BlueprintReadWrite)
		FString To;

	UPROPERTY(BlueprintReadWrite)
		int32 TokenId;

	UPROPERTY(BlueprintReadWrite)
		FString TransactionHash;
};


USTRUCT(BlueprintType)
struct FAlturaItemHistoryEvent
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AlturaSDK")
		FString Id;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AlturaSDK")
		FString Amount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AlturaSDK")
		int32 BlockNumber;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AlturaSDK")
		int32 ChainId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AlturaSDK")
		FString Event;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AlturaSDK")
		FString From;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AlturaSDK")
		FString ItemCollection;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AlturaSDK")
		FString ItemRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AlturaSDK")
		int32 Timestamp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AlturaSDK")
		FString To;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AlturaSDK")
		int32 TokenId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AlturaSDK")
		FString TransactionHash;

	// Optional fields
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AlturaSDK")
		FString Currency;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AlturaSDK")
		float CurrencyPrice;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AlturaSDK")
		float Price;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AlturaSDK")
		float Worth;
};


USTRUCT(BlueprintType)
struct FAlturaUserERC20Balance
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AlturaSDK")
		bool Success;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AlturaSDK")
		float Balance;
};

USTRUCT(BlueprintType)
struct FAlturaUserNativeBalance
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AlturaSDK")
		bool Success;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AlturaSDK")
		float Balance;
};


USTRUCT(BlueprintType)
struct FUserAuth
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AlturaSDK")
	FString Authenticated;
};


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

	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "VerifyAlturaGuardCode", Keywords = "VerifyAlturaGuardCode"), Category = "AlturaSDK | Get Methods")
	static void VerifyAlturaGuardCode(const FString Address, const FString Code, const FResponse& OnComplete);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetUser", Keywords = "GetUser"), Category = "AlturaSDK | Get Methods")
	static void GetUser(const FString Address, const FResponse& OnComplete);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetUsers", Keywords = "GetUsers"), Category = "AlturaSDK | Get Methods")
	static void GetUsers(const FString perPage, const FString page, SortBy SortBy, SortDir Sortdir, UserFilter userFilter, const FString FilterContext, const FResponse& OnComplete);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetItem", Keywords = "GetItem"), Category = "AlturaSDK | Get Methods")
	static void GetItem(const FString Address, const FString TokenId, const FResponse& OnComplete);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetItems", Keywords = "GetItems"), Category = "AlturaSDK | Get Methods")
	static void GetItems(const FString perPage, const FString page, SortBy SortBy, SortDir Sortdir,bool slim,const UserFilter userFilter,const FString FilterContext, const FResponse& OnComplete);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetItemHolders", Keywords = "GetItemHolders"), Category = "AlturaSDK | Get Methods")
	static void GetItemHolders(const FString Address, const FString TokenId,const FString perPage, const FString page, SortBy SortBy, SortDir Sortdir,bool includeListed, const FResponse& OnComplete);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetItemHistory", Keywords = "GetItemHistory"), Category = "AlturaSDK | Get Methods")
	static void GetItemHistory(FString Address, FString TokenId, FString perPage, FString page, const FResponse& OnComplete);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetCollection", Keywords = "GetCollection"), Category = "AlturaSDK | Get Methods")
	static void GetCollection(const FString Address, const FResponse& OnComplete);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetCollections", Keywords = "GetCollections"), Category = "AlturaSDK | Get Methods")
	static void GetCollections(const FString perPage, const FString page, SortBy SortBy, SortDir Sortdir, const FResponse& OnComplete);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetUserItemBalance", Keywords = "GetUserItemBalance"), Category = "AlturaSDK | Get Methods")
	static void GetUserItemBalance(const FString userAddress, const FString collectionAddress, const FString tokenId, const FString chainId, const FResponse& OnComplete);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetUserERC20Balance", Keywords = "GetUserERC20Balance"), Category = "AlturaSDK | Get Methods")
	static void GetUserERC20Balance(const FString chainId, const FString userAddress, const FString tokenAddress, const FResponse& OnComplete);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetUserNativeBalance", Keywords = "GetUserNativeTokenBalance"), Category = "AlturaSDK | Get Methods")
	static void GetUserNativeBalance(const FString chainId, const FString userAddress, const FResponse& OnComplete);


	// Parsing Methods : 

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "To JSON", Keywords = "Response String to JSON"), Category = "AlturaSDK | Helpers")
		static UPARAM(DisplayName = "JSON") FAlturaJSONObject ResponseStringToJSON(const FString ResponseString);


	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Object", Keywords = "Get Object from JSON"), Category = "AlturaSDK | Helpers")
		static UPARAM(DisplayName = "JSON") FAlturaJSONObject GetObjectFromJSONObject(const FAlturaJSONObject JSON, const FString Key);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Object Array", Keywords = "Get Object Array from JSON"), Category = "AlturaSDK |  Helpers")
		static UPARAM(DisplayName = "Array of JSON") TArray<FAlturaJSONObject> GetObjectArrayFromJSONObject(const FAlturaJSONObject JSON, const FString Key);


	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get String Array", Keywords = "Get String Array from JSON"), Category = "AlturaSDK |  Helpers")
		static UPARAM(DisplayName = "Array of Strings") TArray<FString> GetStringArrayFromJSONObject(const FAlturaJSONObject JSON, const FString Key);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Number Array", Keywords = "Get Number Array from JSON"), Category = "AlturaSDK |  Helpers")
		static UPARAM(DisplayName = "Array of Numbers") TArray<float> GetNumberArrayFromJSONObject(const FAlturaJSONObject JSON, const FString Key);


	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Boolean Array", Keywords = "Get Boolean Array from JSON"), Category = "AlturaSDK |  Helpers")
		static UPARAM(DisplayName = "Array of Boolean") TArray<bool> GetBooleanArrayFromJSONObject(const FAlturaJSONObject JSON, const FString Key);


	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get String", Keywords = "Get String from JSON"), Category = "AlturaSDK |  Helpers")
		static UPARAM(DisplayName = "String") FString GetStringFromJSONObject(const FAlturaJSONObject JSON, const FString Key);


	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Number", Keywords = "Get Number from JSON"), Category = "AlturaSDK |  Helpers")
		static UPARAM(DisplayName = "Number") float GetNumberFromJSONObject(const FAlturaJSONObject JSON, const FString Key);


	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Boolean", Keywords = "Get Boolean from JSON"), Category = "AlturaSDK |  Helpers")
		static UPARAM(DisplayName = "Boolean") bool GetBooleanFromJSONObject(const FAlturaJSONObject JSON, const FString Key);



	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Replace With ASCII", Keywords = "Replace With ASCII"), Category = "AlturaSDK |  Helpers")
		static FString ReplaceWithASCII(const FString& Str);


	//Parsing for structures 

	UFUNCTION(BlueprintCallable, Category = "AlturaSDK | Get Methods Parsers")
	static FUsersRoot ParseGetUser(const FString& JSONString);

	UFUNCTION(BlueprintCallable, Category = "AlturaSDK | Get Methods Parsers")
	static FUsersRoot ParseGetUsers(const FString& JSONString);

	UFUNCTION(BlueprintCallable, Category = "AlturaSDK | Get Methods Parsers")
	static FItem ParseItem(const FString& JSONString);

	UFUNCTION(BlueprintCallable, Category = "AlturaSDK | Get Methods Parsers")
	static TArray<FArraySingleItem> ParseItems(const FString& JSONString);


	UFUNCTION(BlueprintCallable, Category = "AlturaSDK | Get Methods Parsers")
	static TArray<FHolder> ParseGetItemHolders(const FString& JSONString);

	UFUNCTION(BlueprintCallable, Category = "AlturaSDK | Get Methods Parsers")
	static TArray<FAlturaItemHistoryEvent> ParseGetItemHistory(const FString& ResponseString);

	UFUNCTION(BlueprintCallable, Category = "AlturaSDK | Get Methods Parsers")
	static FCollection ParseGetCollection(const FString& JSONString);

	UFUNCTION(BlueprintCallable, Category = "AlturaSDK | Get Methods Parsers")
	static TArray<FCollection> ParseGetCollections(const FString& ResponseString);
	
	UFUNCTION(BlueprintCallable, Category = "AlturaSDK | Get Methods Parsers")
	static FUserAuth ParseVerifyAlturaGuardCode(const FString& ResponseString);

	UFUNCTION(BlueprintCallable, Category = "AlturaSDK | Get Methods Parsers")
	static FAlturaUserERC20Balance ParseGetUserERC20Balance(const FString& ResponseString);

	UFUNCTION(BlueprintCallable, Category = "AlturaSDK | Get Methods Parsers")
	static FAlturaUserNativeBalance ParseGetUserNativeBalance(const FString& ResponseString);

};

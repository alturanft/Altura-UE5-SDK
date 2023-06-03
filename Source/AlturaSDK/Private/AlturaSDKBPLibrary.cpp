// Copyright Epic Games, Inc. All Rights Reserved.

#include "AlturaSDKBPLibrary.h"
#include "Json.h"
#include "Dom/JsonObject.h"

#include "AlturaSDK.h"

UAlturaSDKBPLibrary::UAlturaSDKBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

FString UAlturaSDKBPLibrary::SortbyEnumToString(SortBy SortBy)
{
	switch (SortBy)
	{
	case SortBy::address:
		return "address";
	case SortBy::name:
		return "name";
	case SortBy::bio:
		return "bio";
	}
	return "";
}
FString UAlturaSDKBPLibrary::SortDirEnumToString(SortDir SortDir)
{
	switch (SortDir)
	{
	case SortDir::asc:
		return "asc";
	case SortDir::desc:
		return "desc";
	}
	return "";
}
FString UAlturaSDKBPLibrary::UserFilterEnumToString(UserFilter UserFilter)
{
	switch (UserFilter)
	{
	case UserFilter::none:
		return "";
	case UserFilter::address:
		return "address";
	case UserFilter::name:
		return "name";
	case UserFilter::bio:
		return "bio";	
	}
	return "";
}

void UAlturaSDKBPLibrary::MakeAHttpRequest(const FString URL, const TMap<FString, FString> Params, const TMap<FString, FString> Headers, const FString Body, const FResponse& OnComplete)
{


	FHttpModule& HttpModule = FHttpModule::Get();

	// 1. Create Request Object
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = HttpModule.CreateRequest();
	// 2. Set the request method. E.g. GET, POST, PUSH, etc.
	const FString RequestMethod = "GET";
	if (RequestMethod.Len() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Request method not set."));
		return;
	}
	HttpRequest->SetVerb(RequestMethod);


	// 3. Set the url.
	if (URL.Len() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("The URL for the request cannot be empty."));
		return;
	};
	FString FullUrl = URL;


	UE_LOG(LogTemp, Warning, TEXT(" SetURL %s"), *URL);
	HttpRequest->SetURL(URL);


	// 4. Set the request headers
	HttpRequest->SetHeader("Content-Type", "application/json");

	
	for (auto Header : Headers)
	{
		HttpRequest->AppendToHeader(Header.Key, Header.Value);
	}	
	// 5. Add body to the request.
	HttpRequest->SetContentAsString(Body);


	// 6. Attach the callback to notify us that the process is completed.
	HttpRequest->OnProcessRequestComplete().BindLambda(
		// Here, we "capture" the 'this' pointer (the "&"), so our lambda can call this
		// class's methods in the callback.
		[&, OnComplete](
			FHttpRequestPtr Req,
			FHttpResponsePtr Res,
			bool ConnectedSuccessfully) mutable {
				OnComplete.ExecuteIfBound(Res->GetResponseCode(), Res->GetContentAsString());


				UE_LOG(LogTemp, Error, TEXT("HTTP\n %s"), *Res->GetContentAsString());
		});

	// 7. Set Settings
	HttpRequest->SetTimeout(UAlturaSDKBPLibrary::RequestTimeoutDuration);

	// 8. Submit the request for processing
	HttpRequest->ProcessRequest();
}

FString UAlturaSDKBPLibrary::BuildFullUrl(const FString& BaseUrl, const TMap<FString, FString>& Params,int ParsingMethods)
{
	FString FullUrl = BaseUrl;
	int32 ParamIndex = 0;
	switch (ParsingMethods)
	{
	case 1:
		{
			for (const auto& Param : Params)
			{
				if (ParamIndex == 0)
				{
					FullUrl = FullUrl + TEXT("?");
				}
				else
				{
					FullUrl = FullUrl + TEXT("&");
				}
				FullUrl = FullUrl + Param.Key + TEXT("=") + Param.Value;
				ParamIndex = ParamIndex + 1;
			}
			return FullUrl;

			
		}
	case 2 :
		{
			for (const auto& Param : Params)
			{
			
				FullUrl = FullUrl + "/" + Param.Value;
				ParamIndex = ParamIndex + 1;
			}
		}
		return FullUrl;
	}
	return FullUrl;

}
FString UAlturaSDKBPLibrary::AppendToUrlWithSlash(const FString& BaseUrl, const TMap<FString, FString>& Params)
{
	FString FullUrl = BaseUrl;
	int32 ParamIndex = 0;
	for (const auto& Param : Params)
	{
			
		FullUrl = FullUrl + "/" + Param.Value;
		ParamIndex = ParamIndex + 1;
	}
	return FullUrl;

			
}

FString UAlturaSDKBPLibrary::CreateJsonFromParams(const TMap<FString, FString>& Params)
{
	// Create a JSON object from the parameters
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	for (const auto& Param : Params)
	{
		JsonObject->SetStringField(Param.Key, Param.Value);
	}

	// // Serialize the JSON object to a string
	FString JsonString;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);
	UE_LOG(LogTemp, Error, TEXT("Body\n %s"), *JsonString);

	return JsonString;
}
FString UAlturaSDKBPLibrary::AppendToUrlWithQ(const FString& BaseUrl, const TMap<FString, FString>& Params)
{
	FString FullUrl = BaseUrl;
	int32 ParamIndex = 0;
			for (const auto& Param : Params)
			{
				if (ParamIndex == 0)
				{
					FullUrl = FullUrl + TEXT("?");
				}
				else
				{
					FullUrl = FullUrl + TEXT("&");
				}
				FullUrl = FullUrl + Param.Key + TEXT("=") + Param.Value;
				ParamIndex = ParamIndex + 1;
			}
			return FullUrl;

			
		}


void UAlturaSDKBPLibrary::VerifyAlturaGuardCode(const FString Address, const FString Code , const FResponse& OnComplete)
{
    FString url = FString::Printf(TEXT("https://api.alturanft.com/api/v2/user/verify_auth_code/%s/%s"), *Address, *Code);
	MakeAHttpRequest(url, TMap<FString, FString>(), TMap<FString, FString>(), FString(), OnComplete);
}

void UAlturaSDKBPLibrary::GetUser(const FString Address, const FResponse& OnComplete)
{

	TMap<FString, FString> Params;
	Params.Add("address", Address);
	FString url = AppendToUrlWithQ("https://api.alturanft.com/api/v2/user",Params);
	MakeAHttpRequest(url, Params, TMap<FString, FString>(), FString(), OnComplete);

	
}
void UAlturaSDKBPLibrary::GetUsers(const FString perPage, const FString page, SortBy SortBy, SortDir Sortdir,const UserFilter userFilter, const FString FilterContext, const FResponse& OnComplete)
{
	TMap<FString, FString> Params;
	Params.Add("perPage", perPage);
	Params.Add("page", page);
	Params.Add("sortBy", SortbyEnumToString(SortBy));
	Params.Add("sortDir", SortDirEnumToString(Sortdir));
	
	if (userFilter != UserFilter::none)
	{
		Params.Add(UserFilterEnumToString(userFilter), FilterContext);
	}

	FString url = AppendToUrlWithQ("https://api.alturanft.com/api/v2/user",Params);

	MakeAHttpRequest(url, Params, TMap<FString, FString>(), FString(), OnComplete);

	

}

void UAlturaSDKBPLibrary::GetItem(const FString collection_address, const FString token_id, const FResponse& OnComplete)
{
	TMap<FString, FString> Params;
	Params.Add("address", collection_address);
	Params.Add("token_id", token_id);
	FString url = AppendToUrlWithSlash("https://api.alturanft.com/api/v2/item",Params);

	MakeAHttpRequest(url, Params, TMap<FString, FString>(), FString(), OnComplete);

}
void UAlturaSDKBPLibrary::GetItems(const FString perPage, const FString page, SortBy SortBy, SortDir Sortdir,bool slim,const UserFilter userFilter,const FString FilterContext, const FResponse& OnComplete)
{

	TMap<FString, FString> Params;
	Params.Add("perPage", perPage);
	Params.Add("page", page);
	Params.Add("sortBy", SortbyEnumToString(SortBy));
	Params.Add("sortDir", SortDirEnumToString(Sortdir));
	Params.Add("slim", slim ? "true" : "false");

	if (userFilter != UserFilter::none)
	{
		Params.Add(UserFilterEnumToString(userFilter), FilterContext);
	}
	FString url = AppendToUrlWithQ("https://api.alturanft.com/api/v2/item",Params);

	MakeAHttpRequest(url, Params, TMap<FString, FString>(), FString(), OnComplete);

}



void UAlturaSDKBPLibrary::GetItemHolders(const FString Address, const FString TokenId,const FString perPage, const FString page, SortBy SortBy, SortDir Sortdir,bool includeListed, const FResponse& OnComplete)
{

	TMap<FString, FString> Params;
	Params.Add("address", Address);
	Params.Add("tokenId", TokenId);
	FString url = AppendToUrlWithSlash("https://api.alturanft.com/api/v2/item/holders",Params);
	TMap<FString, FString> FilterParams;
	FilterParams.Add("perPage", perPage);
	FilterParams.Add("page", page);
	FilterParams.Add("SortBy", SortbyEnumToString(SortBy));
	FilterParams.Add("Sortdir", SortDirEnumToString(Sortdir));
	FilterParams.Add("includeListed", includeListed ? "true" : "false");
	FString FilterdUrl = AppendToUrlWithQ(url,FilterParams);

	MakeAHttpRequest(FilterdUrl, Params, TMap<FString, FString>(), FString(), OnComplete);
}

void UAlturaSDKBPLibrary::GetItemHistory(const FString Address, const FString TokenId,const FString perPage, const FString page, const FResponse& OnComplete)
{
	TMap<FString, FString> Params;
	Params.Add("Address", Address);
	Params.Add("TokenId", TokenId);
	//Params.Add("perPage", perPage); TODO: Investigate from API service
	//Params.Add("page", page);
	FString url = AppendToUrlWithSlash("https://api.alturanft.com/api/v2/item/events",Params);

	MakeAHttpRequest(url, Params, TMap<FString, FString>(), FString(), OnComplete);
}
void UAlturaSDKBPLibrary::GetCollection(const FString Address, const FResponse& OnComplete)
{
	TMap<FString, FString> Params;
	Params.Add("address", Address);
	FString url = AppendToUrlWithSlash("https://api.alturanft.com/api/v2/collection",Params);

	MakeAHttpRequest(url, Params, TMap<FString, FString>(), FString(), OnComplete);
}
void UAlturaSDKBPLibrary::GetCollections(const FString perPage, const FString page, SortBy SortBy, SortDir Sortdir, const FResponse& OnComplete)
{

	TMap<FString, FString> Params;
	Params.Add("perPage", perPage);
	Params.Add("page", page);
	Params.Add("sortBy", SortbyEnumToString(SortBy));
	Params.Add("sortDir", SortDirEnumToString(Sortdir));
	FString url = AppendToUrlWithQ("https://api.alturanft.com/api/v2/collection",Params);

	MakeAHttpRequest(url, Params, TMap<FString, FString>(), FString(), OnComplete);

}
void UAlturaSDKBPLibrary::GetUserItemBalance(const FString userAddress, const FString collectionAddress, const FString tokenId, const FString chainId, const FResponse& OnComplete)
{
	
	TMap<FString, FString> Params;
	Params.Add("chainId", chainId);
	Params.Add("userAddress", userAddress);
	Params.Add("collectionAddress", collectionAddress);
	Params.Add("tokenId", tokenId);
	FString url = AppendToUrlWithQ("https://api.alturanft.com/api/v2/item/balance",Params);

	MakeAHttpRequest(url, Params, TMap<FString, FString>(), FString(), OnComplete);

}

void UAlturaSDKBPLibrary::GetUserERC20Balance(const FString chainId, const FString userAddress, const FString tokenAddress, const FResponse& OnComplete)
{
	TMap<FString, FString> Params;
	Params.Add("userAddress", userAddress);

	Params.Add("tokenAddress", tokenAddress);
	Params.Add("chainId", chainId);
	FString url = AppendToUrlWithQ("https://api.alturanft.com/api/v2/erc20/balance",Params);

	MakeAHttpRequest(url, Params, TMap<FString, FString>(), FString(), OnComplete);
}

void UAlturaSDKBPLibrary::GetUserNativeBalance(const FString chainId, const FString userAddress, const FResponse& OnComplete)
{
	TMap<FString, FString> Params;
	Params.Add("chainId", chainId);
	Params.Add("userAddress", userAddress);
	FString url = AppendToUrlWithQ("https://api.alturanft.com/api/v2/native/balance",Params);
	
	MakeAHttpRequest(url, Params, TMap<FString, FString>(), FString(), OnComplete);
}





// Parsing helpers 



FAlturaJSONObject UAlturaSDKBPLibrary::ResponseStringToJSON(const FString ResponseString)
{
	FAlturaJSONObject JSONObject = FAlturaJSONObject();
	const TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ResponseString);
	TSharedPtr<FJsonObject> JsonParsed;
	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed)) {
		JSONObject.JsonParsed = JsonParsed;
	}

	return JSONObject;
}

FAlturaJSONObject UAlturaSDKBPLibrary::GetObjectFromJSONObject(const FAlturaJSONObject JSON, const FString Key)
{
	FAlturaJSONObject JsonObject = FAlturaJSONObject();
	if (JSON.JsonParsed != nullptr)
	{

		JsonObject.JsonParsed = JSON.JsonParsed->GetObjectField(Key);
	}
	return JsonObject;
}



TArray<FAlturaJSONObject> UAlturaSDKBPLibrary::GetObjectArrayFromJSONObject(const FAlturaJSONObject JSON, const FString Key)
{
	TArray<FAlturaJSONObject> ObjectArray = TArray<FAlturaJSONObject>();
	if (JSON.JsonParsed != nullptr)
	{
		for (auto ArrayField : JSON.JsonParsed->GetArrayField(Key))
		{
			FAlturaJSONObject TempJsonObject = FAlturaJSONObject();
			TempJsonObject.JsonParsed = ArrayField->AsObject();
			ObjectArray.Add(TempJsonObject);
		}
	}
	return ObjectArray;
}

TArray<FString> UAlturaSDKBPLibrary::GetStringArrayFromJSONObject(const FAlturaJSONObject JSON, const FString Key)
{
	TArray<FString> StringArray = TArray<FString>();
	if (JSON.JsonParsed != nullptr)
	{

		JSON.JsonParsed->TryGetStringArrayField(Key, StringArray);
	}
	return StringArray;
}

TArray<float> UAlturaSDKBPLibrary::GetNumberArrayFromJSONObject(const FAlturaJSONObject JSON, const FString Key)
{
	TArray<float> NumberArray = TArray<float>();
	if (JSON.JsonParsed != nullptr)
	{

		for (auto ArrayField : JSON.JsonParsed->GetArrayField(Key))
		{
			float TempNumber = ArrayField->AsNumber();
			NumberArray.Add(TempNumber);
		}
	}
	return NumberArray;
}

TArray<bool> UAlturaSDKBPLibrary::GetBooleanArrayFromJSONObject(const FAlturaJSONObject JSON, const FString Key)
{
	TArray<bool> BooleanArray = TArray<bool>();
	if (JSON.JsonParsed != nullptr)
	{

		for (auto ArrayField : JSON.JsonParsed->GetArrayField(Key))
		{
			bool TempBool = ArrayField->AsBool();
			BooleanArray.Add(TempBool);
		}
	}
	return BooleanArray;
}

FString UAlturaSDKBPLibrary::GetStringFromJSONObject(const FAlturaJSONObject JSON, const FString Key)
{
	FString StringData = "";
	if (JSON.JsonParsed != nullptr)
	{
		JSON.JsonParsed->TryGetStringField(Key, StringData);
	}
	return StringData;
}

float UAlturaSDKBPLibrary::GetNumberFromJSONObject(const FAlturaJSONObject JSON, const FString Key)
{
	double NumberData = 0;
	if (JSON.JsonParsed != nullptr)
	{
		JSON.JsonParsed->TryGetNumberField(Key, NumberData);
	}
	return NumberData;
}

bool UAlturaSDKBPLibrary::GetBooleanFromJSONObject(const FAlturaJSONObject JSON, const FString Key)
{
	bool BooleanData = false;
	if (JSON.JsonParsed != nullptr)
	{
		JSON.JsonParsed->TryGetBoolField(Key, BooleanData);
	}
	return BooleanData;
}


FString UAlturaSDKBPLibrary::ReplaceWithASCII(const FString& Str)
{

	FString EncodedURL = Str;
	EncodedURL.ReplaceInline(TEXT(" "), TEXT("%20"), ESearchCase::CaseSensitive);
	EncodedURL.ReplaceInline(TEXT("!"), TEXT("%21"), ESearchCase::CaseSensitive);
	EncodedURL.ReplaceInline(TEXT("\""), TEXT("%22"), ESearchCase::CaseSensitive);
	EncodedURL.ReplaceInline(TEXT("#"), TEXT("%23"), ESearchCase::CaseSensitive);
	EncodedURL.ReplaceInline(TEXT("$"), TEXT("%24"), ESearchCase::CaseSensitive);
	EncodedURL.ReplaceInline(TEXT("&"), TEXT("%26"), ESearchCase::CaseSensitive);
	EncodedURL.ReplaceInline(TEXT("'"), TEXT("%27"), ESearchCase::CaseSensitive);
	EncodedURL.ReplaceInline(TEXT("("), TEXT("%28"), ESearchCase::CaseSensitive);
	EncodedURL.ReplaceInline(TEXT(")"), TEXT("%29"), ESearchCase::CaseSensitive);
	EncodedURL.ReplaceInline(TEXT("*"), TEXT("%2A"), ESearchCase::CaseSensitive);
	EncodedURL.ReplaceInline(TEXT("+"), TEXT("%2B"), ESearchCase::CaseSensitive);
	EncodedURL.ReplaceInline(TEXT(","), TEXT("%2C"), ESearchCase::CaseSensitive);
	EncodedURL.ReplaceInline(TEXT("-"), TEXT("%2D"), ESearchCase::CaseSensitive);
	EncodedURL.ReplaceInline(TEXT("."), TEXT("%2E"), ESearchCase::CaseSensitive);
	EncodedURL.ReplaceInline(TEXT("/"), TEXT("%2F"), ESearchCase::CaseSensitive);
	EncodedURL.ReplaceInline(TEXT(":"), TEXT("%3A"), ESearchCase::CaseSensitive);
	EncodedURL.ReplaceInline(TEXT(";"), TEXT("%3B"), ESearchCase::CaseSensitive);
	EncodedURL.ReplaceInline(TEXT("<"), TEXT("%3C"), ESearchCase::CaseSensitive);
	EncodedURL.ReplaceInline(TEXT("="), TEXT("%3D"), ESearchCase::CaseSensitive);
	EncodedURL.ReplaceInline(TEXT(">"), TEXT("%3E"), ESearchCase::CaseSensitive);
	EncodedURL.ReplaceInline(TEXT("?"), TEXT("%3F"), ESearchCase::CaseSensitive);
	EncodedURL.ReplaceInline(TEXT("@"), TEXT("%40"), ESearchCase::CaseSensitive);
	EncodedURL.ReplaceInline(TEXT("["), TEXT("%5B"), ESearchCase::CaseSensitive);
	EncodedURL.ReplaceInline(TEXT("\\"), TEXT("%5C"), ESearchCase::CaseSensitive);
	EncodedURL.ReplaceInline(TEXT("]"), TEXT("%5D"), ESearchCase::CaseSensitive);
	EncodedURL.ReplaceInline(TEXT("^"), TEXT("%5E"), ESearchCase::CaseSensitive);
	EncodedURL.ReplaceInline(TEXT("_"), TEXT("%5F"), ESearchCase::CaseSensitive);
	EncodedURL.ReplaceInline(TEXT("`"), TEXT("%60"), ESearchCase::CaseSensitive);
	EncodedURL.ReplaceInline(TEXT("{"), TEXT("%7B"), ESearchCase::CaseSensitive);
	EncodedURL.ReplaceInline(TEXT("|"), TEXT("%7C"), ESearchCase::CaseSensitive);
	EncodedURL.ReplaceInline(TEXT("}"), TEXT("%7D"), ESearchCase::CaseSensitive);
	EncodedURL.ReplaceInline(TEXT("~"), TEXT("%7E"), ESearchCase::CaseSensitive);
	return EncodedURL;
}

//Parsing for structures 

FUsersRoot UAlturaSDKBPLibrary::ParseGetUser(const FString& ResponseString)
{
	// Convert the response string to a JSON object
	FAlturaJSONObject RootJSON = ResponseStringToJSON(ResponseString);

	// Get the `count` value from the root JSON object
	int32 Count = GetNumberFromJSONObject(RootJSON, "count");

	// Get the `users` array from the root JSON object
	TArray<FAlturaJSONObject> UsersArray = GetObjectArrayFromJSONObject(RootJSON, "users");

	TArray<FAlturaUser> Users;
	for (FAlturaJSONObject UserJSON : UsersArray)
	{
		FAlturaUser User;
		User.Address = GetStringFromJSONObject(UserJSON, "address");
		User.Name = GetStringFromJSONObject(UserJSON, "name");
		User.Bio = GetStringFromJSONObject(UserJSON, "bio");
		User.SocialLink = GetStringFromJSONObject(UserJSON, "socialLink");
		User.ProfilePicUrl = GetStringFromJSONObject(UserJSON, "profilePicUrl");

		Users.Add(User);
	}

	FUsersRoot Root;
	Root.Users = Users;
	Root.Count = Count;

	return Root;
}
FUsersRoot UAlturaSDKBPLibrary::ParseGetUsers(const FString& JSONString)
{
	// Convert the response string to a JSON object
	FAlturaJSONObject RootJSON = ResponseStringToJSON(JSONString);

	// Get the `count` value from the root JSON object
	int32 Count = GetNumberFromJSONObject(RootJSON, "count");

	// Get the `users` array from the root JSON object
	TArray<FAlturaJSONObject> UsersArray = GetObjectArrayFromJSONObject(RootJSON, "users");

	TArray<FAlturaUser> Users;
	for (FAlturaJSONObject UserJSON : UsersArray)
	{
		FAlturaUser User;
		User.Address = GetStringFromJSONObject(UserJSON, "address");
		User.Name = GetStringFromJSONObject(UserJSON, "name");
		User.Bio = GetStringFromJSONObject(UserJSON, "bio");
		User.SocialLink = GetStringFromJSONObject(UserJSON, "socialLink");
		User.ProfilePicUrl = GetStringFromJSONObject(UserJSON, "profilePicUrl");

		Users.Add(User);
	}

	FUsersRoot Root;
	Root.Users = Users;
	Root.Count = Count;

	return Root;
}
FItem UAlturaSDKBPLibrary::ParseItem(const FString& JSONString)
{
	// Convert the response string to a JSON object
	FAlturaJSONObject RootJSON = ResponseStringToJSON(JSONString);

	// Get the `item` object from the root JSON object
	FAlturaJSONObject ItemJSON = GetObjectFromJSONObject(RootJSON, "item");

	FItem Item;
	Item.CollectionAddress = GetStringFromJSONObject(ItemJSON, "collectionAddress");
	Item.TokenId = GetNumberFromJSONObject(ItemJSON, "tokenId");
	Item.ChainId = GetNumberFromJSONObject(ItemJSON, "chainId");
	Item.Name = GetStringFromJSONObject(ItemJSON, "name");
	Item.Description = GetStringFromJSONObject(ItemJSON, "description");
	Item.Image = GetStringFromJSONObject(ItemJSON, "image");
	Item.ImageUrl = GetStringFromJSONObject(ItemJSON, "imageUrl");
	Item.FileType = GetStringFromJSONObject(ItemJSON, "fileType");
	Item.bIsVideo = GetBooleanFromJSONObject(ItemJSON, "isVideo");
	Item.CreatorAddress = GetStringFromJSONObject(ItemJSON, "creatorAddress");
	Item.Likes = GetNumberFromJSONObject(ItemJSON, "likes");
	Item.Views = GetNumberFromJSONObject(ItemJSON, "views");
	Item.MintDate = GetStringFromJSONObject(ItemJSON, "mintDate");
	Item.Royalty = GetNumberFromJSONObject(ItemJSON, "royalty");
	Item.bIsNSFW = GetBooleanFromJSONObject(ItemJSON, "nsfw");
	Item.Supply = GetNumberFromJSONObject(ItemJSON, "supply");
	Item.MaxSupply = GetNumberFromJSONObject(ItemJSON, "maxSupply");
	Item.bIsStackable = GetBooleanFromJSONObject(ItemJSON, "stackable");

	TArray<FAlturaJSONObject> PropertiesArray = GetObjectArrayFromJSONObject(ItemJSON, "properties");
	for (FAlturaJSONObject PropertyJSON : PropertiesArray)
	{
		FItemProperty Property;
		Property.Id = GetStringFromJSONObject(PropertyJSON, "_id");
		Property.Name = GetStringFromJSONObject(PropertyJSON, "name");
		Property.Value = GetStringFromJSONObject(PropertyJSON, "value");
		Property.bStatic = GetBooleanFromJSONObject(PropertyJSON, "static");

		Item.Properties.Add(Property);
	}

	Item.OtherImageVisibility = GetStringFromJSONObject(ItemJSON, "otherImageVisibility");
	Item.bIsListed = GetBooleanFromJSONObject(ItemJSON, "isListed");
	Item.MostRecentListing = GetStringFromJSONObject(ItemJSON, "mostRecentListing");
	Item.CheapestListingUSD = GetNumberFromJSONObject(ItemJSON, "cheapestListingUSD");
	Item.CheapestListingPrice = GetStringFromJSONObject(ItemJSON, "cheapestListingPrice");
	Item.CheapestListingCurrency = GetStringFromJSONObject(ItemJSON, "cheapestListingCurrency");
	Item.bIsLootBoxKey = GetBooleanFromJSONObject(ItemJSON, "isLootBoxKey");
	Item.bIsSmartNFT = GetBooleanFromJSONObject(ItemJSON, "isSmartNFT");
	Item.Holders = GetNumberFromJSONObject(ItemJSON, "holders");
	Item.bIsFeatured = GetBooleanFromJSONObject(ItemJSON, "featured");
	Item.bHasUnlockableContent = GetBooleanFromJSONObject(ItemJSON, "hasUnlockableContent");
	Item.bDidLike = GetBooleanFromJSONObject(ItemJSON, "didLike");
	Item.TotalListings = GetNumberFromJSONObject(ItemJSON, "totalListings");
	Item.CheapestPriceUSD = GetNumberFromJSONObject(ItemJSON, "cheapestPriceUSD");
	Item.CreatorName = GetStringFromJSONObject(ItemJSON, "creatorName");
	Item.CreatorProfilePic = GetStringFromJSONObject(ItemJSON, "creatorProfilePic");
	Item.CollectionName = GetStringFromJSONObject(ItemJSON, "collectionName");
	Item.URI = GetStringFromJSONObject(ItemJSON, "uri");
	Item.bIsVerified = GetBooleanFromJSONObject(ItemJSON, "isVerified");
	Item.Genre = GetStringFromJSONObject(ItemJSON, "genre");
	Item.Website = GetStringFromJSONObject(ItemJSON, "website");
	Item.Version = GetNumberFromJSONObject(ItemJSON, "version");
	Item.CollectionImage = GetStringFromJSONObject(ItemJSON, "collectionImage");
	Item.CollectionImageUrl = GetStringFromJSONObject(ItemJSON, "collectionImageUrl");
	Item.Slug = GetStringFromJSONObject(ItemJSON, "slug");

	return Item;
}
TArray<FArraySingleItem> UAlturaSDKBPLibrary::ParseItems(const FString& JSONString)
{
	TArray<FArraySingleItem> ResultItems;

	FAlturaJSONObject JSONObject = ResponseStringToJSON(JSONString);

	TArray<FAlturaJSONObject> ItemsArray = GetObjectArrayFromJSONObject(JSONObject, "items");

	for (const FAlturaJSONObject& ItemObject : ItemsArray)
	{
		FArraySingleItem Item;

		Item.Name = GetStringFromJSONObject(ItemObject, "name");

		TArray<FAlturaJSONObject> PropertiesArray = GetObjectArrayFromJSONObject(ItemObject, "properties");
		for (const FAlturaJSONObject& PropertyObject : PropertiesArray)
		{
			FItemProperty Property;
			Property.Id = GetStringFromJSONObject(PropertyObject, "_id");
			Property.Name = GetStringFromJSONObject(PropertyObject, "name");
			Property.Value = GetStringFromJSONObject(PropertyObject, "value");
			Property.bStatic = GetBooleanFromJSONObject(PropertyObject, "static");
			Item.Properties.Add(Property);
		}

		Item.TokenId = GetNumberFromJSONObject(ItemObject, "tokenId");
		Item.CollectionAddress = GetStringFromJSONObject(ItemObject, "collectionAddress");
		Item.Supply = GetNumberFromJSONObject(ItemObject, "supply");
		Item.MaxSupply = GetNumberFromJSONObject(ItemObject, "maxSupply");
		Item.Image = GetStringFromJSONObject(ItemObject, "image");
		Item.ImageUrl = GetStringFromJSONObject(ItemObject, "imageUrl");
		Item.ImageIndex = GetNumberFromJSONObject(ItemObject, "imageIndex");
		Item.ImageCount = GetNumberFromJSONObject(ItemObject, "imageCount");

		ResultItems.Add(Item);
	}

	return ResultItems;
}
TArray<FHolder> UAlturaSDKBPLibrary::ParseGetItemHolders(const FString& JSONString)
{
	TArray<FHolder> ResultHolders;

	FAlturaJSONObject JSONObject = ResponseStringToJSON(JSONString);

	TArray<FAlturaJSONObject> HoldersArray = GetObjectArrayFromJSONObject(JSONObject, "holders");

	for (const FAlturaJSONObject& HolderObject : HoldersArray)
	{
		FHolder Holder;

		Holder.Address = GetStringFromJSONObject(HolderObject, "address");
		Holder.Balance = static_cast<int32>(GetNumberFromJSONObject(HolderObject, "balance"));  // Cast float to int32
		Holder.Name = GetStringFromJSONObject(HolderObject, "name");
		Holder.ProfilePic = GetStringFromJSONObject(HolderObject, "profilePic");
		Holder.ProfilePicUrl = GetStringFromJSONObject(HolderObject, "profilePicUrl");

		ResultHolders.Add(Holder);
	}

	return ResultHolders;
}
TArray<FAlturaItemHistoryEvent> UAlturaSDKBPLibrary::ParseGetItemHistory(const FString& ResponseString)
{
	TArray<FAlturaItemHistoryEvent> ItemHistory;

	FAlturaJSONObject JSONObject = ResponseStringToJSON(ResponseString);
	TArray<FAlturaJSONObject> Events = GetObjectArrayFromJSONObject(JSONObject, "events");

	for (FAlturaJSONObject Event : Events)
	{
		FAlturaItemHistoryEvent HistoryEvent;
		HistoryEvent.Id = GetStringFromJSONObject(Event, "id");
		HistoryEvent.Amount = GetStringFromJSONObject(Event, "amount");
		HistoryEvent.BlockNumber = GetNumberFromJSONObject(Event, "blockNumber");
		HistoryEvent.ChainId = GetNumberFromJSONObject(Event, "chainId");
		HistoryEvent.Event = GetStringFromJSONObject(Event, "event");
		HistoryEvent.From = GetStringFromJSONObject(Event, "from");
		HistoryEvent.ItemCollection = GetStringFromJSONObject(Event, "itemCollection");
		HistoryEvent.ItemRef = GetStringFromJSONObject(Event, "itemRef");
		HistoryEvent.Timestamp = GetNumberFromJSONObject(Event, "timestamp");
		HistoryEvent.To = GetStringFromJSONObject(Event, "to");
		HistoryEvent.TokenId = GetNumberFromJSONObject(Event, "tokenId");
		HistoryEvent.TransactionHash = GetStringFromJSONObject(Event, "transactionHash");
		// Optional fields
		HistoryEvent.Currency = GetStringFromJSONObject(Event, "currency");
		HistoryEvent.CurrencyPrice = GetNumberFromJSONObject(Event, "currencyPrice");
		HistoryEvent.Price = GetNumberFromJSONObject(Event, "price");
		HistoryEvent.Worth = GetNumberFromJSONObject(Event, "worth");

		ItemHistory.Add(HistoryEvent);
	}

	return ItemHistory;
}
FCollection UAlturaSDKBPLibrary::ParseGetCollection(const FString& ResponseString)
{
	FCollection ResultCollection;

	FAlturaJSONObject JSONObject = ResponseStringToJSON(ResponseString);
	FAlturaJSONObject CollectionObject = GetObjectFromJSONObject(JSONObject, "collection");

	ResultCollection.Address = GetStringFromJSONObject(CollectionObject, "address");
	ResultCollection.Name = GetStringFromJSONObject(CollectionObject, "name");
	ResultCollection.Description = GetStringFromJSONObject(CollectionObject, "description");
	ResultCollection.Genre = GetStringFromJSONObject(CollectionObject, "genre");
	ResultCollection.Image = GetStringFromJSONObject(CollectionObject, "image");
	ResultCollection.ImageHash = GetStringFromJSONObject(CollectionObject, "imageHash");
	ResultCollection.OwnerAddress = GetStringFromJSONObject(CollectionObject, "ownerAddress");
	ResultCollection.Slug = GetStringFromJSONObject(CollectionObject, "slug");
	ResultCollection.Uri = GetStringFromJSONObject(CollectionObject, "uri");
	ResultCollection.Website = GetStringFromJSONObject(CollectionObject, "website");
	ResultCollection.MintDate = GetStringFromJSONObject(CollectionObject, "mintDate");
	ResultCollection.ChainId = FMath::RoundToInt(GetNumberFromJSONObject(CollectionObject, "chainId"));
	ResultCollection.Holders = FMath::RoundToInt(GetNumberFromJSONObject(CollectionObject, "holders"));
	ResultCollection.Volume_1d = GetNumberFromJSONObject(CollectionObject, "volume_1d");
	ResultCollection.Volume_1w = GetNumberFromJSONObject(CollectionObject, "volume_1w");
	ResultCollection.Volume_30d = GetNumberFromJSONObject(CollectionObject, "volume_30d");
	ResultCollection.Volume_all = GetNumberFromJSONObject(CollectionObject, "volume_all");
	ResultCollection.ImageUrl = GetStringFromJSONObject(CollectionObject, "imageUrl");

	return ResultCollection;
}
TArray<FCollection> UAlturaSDKBPLibrary::ParseGetCollections(const FString& ResponseString)
{
	TArray<FCollection> ResultCollections;

	FAlturaJSONObject JSONObject = ResponseStringToJSON(ResponseString);
	TArray<FAlturaJSONObject> CollectionsArray = GetObjectArrayFromJSONObject(JSONObject, "collections");

	for (const FAlturaJSONObject& CollectionObject : CollectionsArray)
	{
		FCollection TempCollection;

		TempCollection.Address = GetStringFromJSONObject(CollectionObject, "address");
		TempCollection.Name = GetStringFromJSONObject(CollectionObject, "name");
		TempCollection.Description = GetStringFromJSONObject(CollectionObject, "description");
		TempCollection.Genre = GetStringFromJSONObject(CollectionObject, "genre");
		TempCollection.Image = GetStringFromJSONObject(CollectionObject, "image");
		TempCollection.ImageHash = GetStringFromJSONObject(CollectionObject, "imageHash");
		TempCollection.OwnerAddress = GetStringFromJSONObject(CollectionObject, "ownerAddress");
		TempCollection.Slug = GetStringFromJSONObject(CollectionObject, "slug");
		TempCollection.Uri = GetStringFromJSONObject(CollectionObject, "uri");
		TempCollection.Website = GetStringFromJSONObject(CollectionObject, "website");
		TempCollection.MintDate = GetStringFromJSONObject(CollectionObject, "mintDate");
		TempCollection.ChainId = FMath::RoundToInt(GetNumberFromJSONObject(CollectionObject, "chainId"));
		TempCollection.Holders = FMath::RoundToInt(GetNumberFromJSONObject(CollectionObject, "holders"));
		TempCollection.Volume_1d = GetNumberFromJSONObject(CollectionObject, "volume_1d");
		TempCollection.Volume_1w = GetNumberFromJSONObject(CollectionObject, "volume_1w");
		TempCollection.Volume_30d = GetNumberFromJSONObject(CollectionObject, "volume_30d");
		TempCollection.Volume_all = GetNumberFromJSONObject(CollectionObject, "volume_all");

		ResultCollections.Add(TempCollection);
	}

	return ResultCollections;
}
FUserAuth UAlturaSDKBPLibrary::ParseVerifyAlturaGuardCode(const FString& ResponseString)
{
	FUserAuth UserAuth;

	FAlturaJSONObject JSONObject = ResponseStringToJSON(ResponseString);   
	FString AuthenticatedString = GetStringFromJSONObject(JSONObject, "authenticated");
	bool AuthenticatedBool = (AuthenticatedString.ToLower() == "true");
	UserAuth.Authenticated = AuthenticatedBool ? "true" : "false";

	return UserAuth;
}


FAlturaUserERC20Balance UAlturaSDKBPLibrary::ParseGetUserERC20Balance(const FString& ResponseString)
{
	FAlturaUserERC20Balance UserERC20Balance;

	FAlturaJSONObject JSONObject = ResponseStringToJSON(ResponseString);
	UserERC20Balance.Success = GetBooleanFromJSONObject(JSONObject, "success");
	UserERC20Balance.Balance = GetNumberFromJSONObject(JSONObject, "message");

	return UserERC20Balance;
}



FAlturaUserNativeBalance UAlturaSDKBPLibrary::ParseGetUserNativeBalance(const FString& ResponseString)
{
	FAlturaUserNativeBalance FAlturaUserNativeBalance;

	FAlturaJSONObject JSONObject = ResponseStringToJSON(ResponseString);
	FAlturaUserNativeBalance.Success = GetBooleanFromJSONObject(JSONObject, "success");
	FAlturaUserNativeBalance.Balance = GetNumberFromJSONObject(JSONObject, "message");

	return FAlturaUserNativeBalance;
}
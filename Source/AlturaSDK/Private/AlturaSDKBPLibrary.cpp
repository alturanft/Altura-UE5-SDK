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
	TMap<FString, FString> Params;
	Params.Add("address", Address);
	Params.Add("code", Code);
	FString url = AppendToUrlWithQ("https://api.alturanft.com/api/v2/user/verify_auth_code",Params);

	MakeAHttpRequest(url, Params, TMap<FString, FString>(), FString(), OnComplete);
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
	// Params.Add("userFilter", UserFilterEnumToString(userFilter));
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
	// Params.Add("perPage", perPage);
	// Params.Add("page", page);
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

void UAlturaSDKBPLibrary::GetUserNativeTokenBalance(const FString chainId, const FString userAddress, const FResponse& OnComplete)
{
	TMap<FString, FString> Params;
	Params.Add("chainId", chainId);
	Params.Add("userAddress", userAddress);
	FString url = AppendToUrlWithQ("https://api.alturanft.com/api/v2/native/balance",Params);
	CreateJsonFromParams(Params);
	
	MakeAHttpRequest(url, TMap<FString, FString>(), TMap<FString, FString>(), CreateJsonFromParams(Params), OnComplete);
}
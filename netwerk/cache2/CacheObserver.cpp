/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "CacheObserver.h"

#include "CacheStorageService.h"
#include "CacheFileIOManager.h"
#include "nsIObserverService.h"
#include "mozilla/Services.h"
#include "mozilla/Preferences.h"
#include "nsServiceManagerUtils.h"

namespace mozilla {
namespace net {

CacheObserver* CacheObserver::sSelf = nullptr;

static uint32_t const kDefaultMemoryLimit = 50 * 1024; // 50 MB
uint32_t CacheObserver::sMemoryLimit = kDefaultMemoryLimit;

static uint32_t const kDefaultUseNewCache = 0; // Don't use the new cache by default
uint32_t CacheObserver::sUseNewCache = kDefaultUseNewCache;

NS_IMPL_ISUPPORTS2(CacheObserver,
                   nsIObserver,
                   nsISupportsWeakReference)

nsresult
CacheObserver::Init()
{
  if (sSelf) {
    return NS_OK;
  }

  nsCOMPtr<nsIObserverService> obs = mozilla::services::GetObserverService();
  if (!obs) {
    return NS_ERROR_UNEXPECTED;
  }

  sSelf = new CacheObserver();
  NS_ADDREF(sSelf);

  obs->AddObserver(sSelf, "prefservice:after-app-defaults", true);
  obs->AddObserver(sSelf, "profile-do-change", true);
  obs->AddObserver(sSelf, "profile-before-change", true);
  obs->AddObserver(sSelf, "xpcom-shutdown", true);
  obs->AddObserver(sSelf, "last-pb-context-exited", true);
  obs->AddObserver(sSelf, "memory-pressure", true);

  return NS_OK;
}

nsresult
CacheObserver::Shutdown()
{
  if (!sSelf) {
    return NS_ERROR_NOT_INITIALIZED;
  }

  NS_RELEASE(sSelf);
  return NS_OK;
}

void
CacheObserver::AttachToPreferences()
{
  mozilla::Preferences::AddUintVarCache(
    &sMemoryLimit, "browser.cache.memory_limit", kDefaultMemoryLimit);
  mozilla::Preferences::AddUintVarCache(
    &sUseNewCache, "browser.cache.use_new_backend", kDefaultUseNewCache);
}

// static
bool const CacheObserver::UseNewCache()
{
  switch (sUseNewCache) {
    case 0: // use the old cache backend
      return false;

    case 1: // use the new cache backend
      return true;

    case 2: // use A/B testing
    {
      static bool const sABTest = rand() & 1;
      return sABTest;
    }
  }

  return true;
}

NS_IMETHODIMP
CacheObserver::Observe(nsISupports* aSubject,
                       const char* aTopic,
                       const PRUnichar* aData)
{
  if (!strcmp(aTopic, "prefservice:after-app-defaults")) {
    CacheFileIOManager::Init();
    return NS_OK;
  }

  if (!strcmp(aTopic, "profile-do-change")) {
    CacheFileIOManager::Init();
    CacheFileIOManager::OnProfile();
    AttachToPreferences();
    return NS_OK;
  }

  if (!strcmp(aTopic, "profile-before-change")) {
    nsRefPtr<CacheStorageService> service = CacheStorageService::Self();
    if (service)
      service->Shutdown();

    return NS_OK;
  }

  if (!strcmp(aTopic, "xpcom-shutdown")) {
    nsRefPtr<CacheStorageService> service = CacheStorageService::Self();
    if (service)
      service->Shutdown();

    CacheFileIOManager::Shutdown();
    return NS_OK;
  }

  if (!strcmp(aTopic, "last-pb-context-exited")) {
    nsRefPtr<CacheStorageService> service = CacheStorageService::Self();
    if (service)
      service->DropPrivateBrowsingEntries();

    return NS_OK;
  }

  if (!strcmp(aTopic, "memory-pressure")) {
    nsRefPtr<CacheStorageService> service = CacheStorageService::Self();
    if (service)
      service->PurgeFromMemory(nsICacheStorageService::PURGE_EVERYTHING);

    return NS_OK;
  }

  return NS_OK;
}

} // net
} // mozilla

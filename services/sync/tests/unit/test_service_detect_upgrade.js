Cu.import("resource://services-sync/main.js");
Cu.import("resource://services-sync/service.js");
Cu.import("resource://services-sync/engines.js");
Cu.import("resource://services-sync/util.js");
Cu.import("resource://services-sync/status.js");
Cu.import("resource://services-sync/constants.js");
Cu.import("resource://services-sync/base_records/wbo.js");      // For Records.
Cu.import("resource://services-sync/base_records/crypto.js");   // For CollectionKeys.
Cu.import("resource://services-sync/engines/tabs.js");
Cu.import("resource://services-sync/log4moz.js");
  
function run_test() {
  let passphrase = "abcdeabcdeabcdeabcdeabcdea";
  let logger = Log4Moz.repository.rootLogger;
  Log4Moz.repository.rootLogger.addAppender(new Log4Moz.DumpAppender());

  let clients = new ServerCollection();
  let meta_global = new ServerWBO('global');

  // Tracking info/collections.
  let collectionsHelper = track_collections_helper();
  let upd = collectionsHelper.with_updated_collection;
  let collections = collectionsHelper.collections;

  do_test_pending();
  let keysWBO = new ServerWBO("keys");
  let server = httpd_setup({
    // Special.
    "/1.0/johndoe/info/collections": collectionsHelper.handler,
    "/1.0/johndoe/storage/crypto/keys": upd("crypto", keysWBO.handler()),
    "/1.0/johndoe/storage/meta/global": upd("meta", meta_global.handler()),
      
    // Track modified times.
    "/1.0/johndoe/storage/clients": upd("clients", clients.handler()),
    "/1.0/johndoe/storage/tabs": upd("tabs", new ServerCollection().handler()),
    
    // Just so we don't get 404s in the logs.
    "/1.0/johndoe/storage/bookmarks": new ServerCollection().handler(),
    "/1.0/johndoe/storage/forms": new ServerCollection().handler(),
    "/1.0/johndoe/storage/history": new ServerCollection().handler(),
    "/1.0/johndoe/storage/passwords": new ServerCollection().handler(),
    "/1.0/johndoe/storage/prefs": new ServerCollection().handler()
  });

  try {
    
    Svc.Prefs.set("registerEngines", "Tab");
    _("Set up some tabs.");
    let myTabs = 
      {windows: [{tabs: [{index: 1,
                          entries: [{
                            url: "http://foo.com/",
                            title: "Title"
                          }],
                          attributes: {
                            image: "image"
                          },
                          extData: {
                            weaveLastUsed: 1
                          }}]}]};
    delete Svc.Session;
    Svc.Session = {
      getBrowserState: function () JSON.stringify(myTabs)
    };
    
    Weave.Service._registerEngines();
    
    _("Logging in.");
    Weave.Service.serverURL = "http://localhost:8080/";
    Weave.Service.clusterURL = "http://localhost:8080/";
    
    Weave.Service.login("johndoe", "ilovejane", passphrase);
    do_check_true(Weave.Service.isLoggedIn);
    Weave.Service.verifyAndFetchSymmetricKeys();
    do_check_true(Weave.Service._remoteSetup());

    function test_out_of_date() {
      _("Old meta/global: " + JSON.stringify(meta_global));
      meta_global.payload = JSON.stringify({"syncID": "foooooooooooooooooooooooooo",
                                            "storageVersion": STORAGE_VERSION + 1});
      collections.meta = Date.now() / 1000;
      _("New meta/global: " + JSON.stringify(meta_global));
      Records.set(Weave.Service.metaURL, meta_global);
      try {
        Weave.Service.sync();
      }
      catch (ex) {
      }
      do_check_eq(Status.sync, VERSION_OUT_OF_DATE);
    }
    
    // See what happens when we bump the storage version.
    _("Syncing after server has been upgraded.");
    test_out_of_date();
    
    // Same should happen after a wipe.
    _("Syncing after server has been upgraded and wiped.");
    Weave.Service.wipeServer();
    test_out_of_date();
    
    // Now's a great time to test what happens when keys get replaced.
    _("Syncing afresh...");
    Weave.Service.logout();
    CollectionKeys.clear();
    Weave.Service.serverURL = "http://localhost:8080/";
    Weave.Service.clusterURL = "http://localhost:8080/";
    meta_global.payload = JSON.stringify({"syncID": "foooooooooooooobbbbbbbbbbbb",
                                          "storageVersion": STORAGE_VERSION});
    collections.meta = Date.now() / 1000;
    Records.set(Weave.Service.metaURL, meta_global);
    Weave.Service.login("johndoe", "ilovejane", passphrase);
    do_check_true(Weave.Service.isLoggedIn);
    Weave.Service.sync();
    do_check_true(Weave.Service.isLoggedIn);
    
    let serverDecrypted;
    let serverKeys;
    let serverResp;
    
    function retrieve_server_default() {
      serverKeys = serverResp = serverDecrypted = null;
      
      serverKeys = new CryptoWrapper("crypto", "keys");
      serverResp = serverKeys.fetch(Weave.Service.cryptoKeysURL).response;
      do_check_true(serverResp.success);
      
      serverDecrypted = serverKeys.decrypt(Weave.Service.syncKeyBundle);
      _("Retrieved WBO:       " + JSON.stringify(serverDecrypted));
      _("serverKeys:          " + JSON.stringify(serverKeys));
      
      return serverDecrypted.default;
    }
      
    function retrieve_and_compare_default(should_succeed) {
      let serverDefault = retrieve_server_default();
      let localDefault = CollectionKeys.keyForCollection().keyPair;
      
      _("Retrieved keyBundle: " + JSON.stringify(serverDefault));
      _("Local keyBundle:     " + JSON.stringify(localDefault));
      
      if (should_succeed)
        do_check_eq(JSON.stringify(serverDefault), JSON.stringify(localDefault));
      else
        do_check_neq(JSON.stringify(serverDefault), JSON.stringify(localDefault));
    }
    
    // Uses the objects set above.
    function set_server_keys(pair) {
      serverDecrypted.default = pair;
      serverKeys.cleartext = serverDecrypted;
      serverKeys.encrypt(Weave.Service.syncKeyBundle);
      serverKeys.upload(Weave.Service.cryptoKeysURL);
    }
    
    _("Checking we have the latest keys.");
    retrieve_and_compare_default(true);
    
    _("Update keys on server.");
    set_server_keys(["KaaaaaaaaaaaHAtfmuRY0XEJ7LXfFuqvF7opFdBD/MY=",
                     "aaaaaaaaaaaapxMO6TEWtLIOv9dj6kBAJdzhWDkkkis="]);
    
    _("Checking that we no longer have the latest keys.");
    retrieve_and_compare_default(false);
    
    _("Indeed, they're what we set them to...");
    do_check_eq("KaaaaaaaaaaaHAtfmuRY0XEJ7LXfFuqvF7opFdBD/MY=",
                retrieve_server_default()[0]);
    
    _("Sync. Should download changed keys automatically.");
    let oldClientsModified = collections.clients;
    let oldTabsModified = collections.tabs;
    
    Weave.Service.login("johndoe", "ilovejane", passphrase);
    Weave.Service.sync();
    _("New key should have forced upload of data.");
    _("Tabs: " + oldTabsModified + " < " + collections.tabs);
    _("Clients: " + oldClientsModified + " < " + collections.clients);
    do_check_true(collections.clients > oldClientsModified);
    do_check_true(collections.tabs    > oldTabsModified);
    
    _("... and keys will now match.");
    retrieve_and_compare_default(true);

  } finally {
    Weave.Svc.Prefs.resetBranch("");
    server.stop(do_test_finished);
  }
}

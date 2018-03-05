#ifndef CONFIG_H_
#define CONFIG_H_

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/uuid/uuid.hpp>             // uuid class
#include <boost/uuid/uuid_generators.hpp>  // generators
#include <boost/uuid/uuid_io.hpp>

#include "logging.h"
#include "types.h"
#include "uptane/secondaryconfig.h"

enum ProvisionMode { kAutomatic = 0, kImplicit };
enum CryptoSource { kFile = 0, kPkcs11 };
enum StorageType { kFileSystem = 0, kSqlite };
enum PackageManager { kNone = 0, kOstree, kDebian };

std::ostream& operator<<(std::ostream& os, CryptoSource cs);
// Keep the order of config options the same as in writeToFile() and
// updateFromPropertyTree() in config.cc.

struct GatewayConfig {
  bool socket{false};
};

struct NetworkConfig {
  std::string socket_commands_path{"/tmp/sota-commands.socket"};
  std::string socket_events_path{"/tmp/sota-events.socket"};
  std::vector<std::string> socket_events{"DownloadComplete", "DownloadFailed"};
};

struct P11Config {
  boost::filesystem::path module;
  std::string pass;
  std::string uptane_key_id;
  std::string tls_cacert_id;
  std::string tls_pkey_id;
  std::string tls_clientcert_id;
};

struct TlsConfig {
  std::string server;
  boost::filesystem::path server_url_path;
  CryptoSource ca_source{kFile};
  CryptoSource pkey_source{kFile};
  CryptoSource cert_source{kFile};
};

struct ProvisionConfig {
  std::string server;
  std::string p12_password;
  std::string expiry_days{"36000"};
  boost::filesystem::path provision_path;
  ProvisionMode mode{kAutomatic};
};

struct UptaneConfig {
  bool polling{true};
  unsigned long long polling_sec{10u};
  std::string device_id;
  std::string primary_ecu_serial;
  std::string primary_ecu_hardware_id;
  std::string director_server;
  std::string repo_server;
  CryptoSource key_source{kFile};
  KeyType key_type{kRSA2048};
  std::vector<Uptane::SecondaryConfig> secondary_configs{};

  std::string getKeyTypeString() const { return (key_type == kED25519) ? "ED25519" : "RSA"; }
};

struct PackageConfig {
  PackageManager type{kOstree};
  std::string os;
  boost::filesystem::path sysroot;
  std::string ostree_server;
  boost::filesystem::path packages_file{"/usr/package.manifest"};
};

struct StorageConfig {
  StorageType type{kFileSystem};
  boost::filesystem::path path{"/var/sota"};
  // TODO: merge with path once SQLStorage class is complete
  boost::filesystem::path sqldb_path{"/var/sota/storage.db"};
  // FS storage
  boost::filesystem::path uptane_metadata_path{"metadata"};
  boost::filesystem::path uptane_private_key_path{"ecukey.pem"};
  boost::filesystem::path uptane_public_key_path{"ecukey.pub"};
  boost::filesystem::path tls_cacert_path{"ca.pem"};
  boost::filesystem::path tls_pkey_path{"pkey.pem"};
  boost::filesystem::path tls_clientcert_path{"client.pem"};

  // SQLite storage
  boost::filesystem::path schemas_path{"/usr/lib/sota/schemas"};
};

struct ImportConfig {
  boost::filesystem::path uptane_private_key_path;
  boost::filesystem::path uptane_public_key_path;
  boost::filesystem::path tls_cacert_path;
  boost::filesystem::path tls_pkey_path;
  boost::filesystem::path tls_clientcert_path;
};

class Config {
 public:
  Config();
  Config(const boost::filesystem::path& filename, const boost::program_options::variables_map& cmd);
  Config(const boost::filesystem::path& filename);

  void updateFromTomlString(const std::string& contents);
  void postUpdateValues();
  void writeToFile(const boost::filesystem::path& filename);

  // config data structures
  GatewayConfig gateway;
  NetworkConfig network;
  P11Config p11;
  TlsConfig tls;
  ProvisionConfig provision;
  UptaneConfig uptane;
  PackageConfig pacman;
  StorageConfig storage;
  ImportConfig import;

 private:
  void updateFromPropertyTree(const boost::property_tree::ptree& pt);
  void updateFromToml(const boost::filesystem::path& filename);
  void updateFromCommandLine(const boost::program_options::variables_map& cmd);
  void readSecondaryConfigs(const std::vector<boost::filesystem::path>& sconfigs);
  void checkLegacyVersion(const boost::filesystem::path& legacy_interface);
  void initLegacySecondaries(const boost::filesystem::path& legacy_interface);
};

#endif  // CONFIG_H_

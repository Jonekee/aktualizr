#ifndef AKTUALIZR_SECONDARY_COMMON_H_
#define AKTUALIZR_SECONDARY_COMMON_H_

#include <map>
#include <mutex>
#include <string>

#include "aktualizr_secondary_config.h"
#include "crypto/keymanager.h"
#include "package_manager/packagemanagerinterface.h"
#include "storage/invstorage.h"

class AktualizrSecondaryCommon {
 public:
  AktualizrSecondaryCommon(const AktualizrSecondaryConfig& /*config*/, std::shared_ptr<INvStorage> /*storage*/);

  bool uptaneInitialize();

  AktualizrSecondaryConfig config_;

  std::shared_ptr<INvStorage> storage_;
  KeyManager keys_;
  Uptane::EcuSerial ecu_serial_;
  Uptane::HardwareIdentifier hardware_id_;
  std::shared_ptr<PackageManagerInterface> pacman;
  Uptane::Root root_;
  Uptane::Targets meta_targets_;
  std::string detected_attack_;
  std::unique_ptr<Uptane::Target> target_;
};

#endif  // AKTUALIZR_SECONDARY_COMMON_H_

#ifndef SOTA_CLIENT_TOOLS_OSTREE_REPO_H_
#define SOTA_CLIENT_TOOLS_OSTREE_REPO_H_

#include <boost/filesystem.hpp>

#include "garage_common.h"
#include "ostree_hash.h"
#include "ostree_object.h"

class OSTreeRef;

/**
 * A source repository to read OSTree objects from. This can be either a directory
 * on disk, or a URL in the garage-deploy case.
 */
class OSTreeRepo {
 public:
  using ptr = std::shared_ptr<OSTreeRepo>;
  // OSTreeRepo(const OSTreeRepo&) = delete;
  OSTreeRepo& operator=(const OSTreeRepo&) = delete;

  virtual ~OSTreeRepo() = default;
  virtual bool LooksValid() const = 0;
  virtual OSTreeObject::ptr GetObject(OSTreeHash hash, OstreeObjectType type) const = 0;
  virtual OSTreeObject::ptr GetObject(const uint8_t sha256[32], OstreeObjectType type) const = 0;
  virtual const boost::filesystem::path root() const = 0;
  virtual OSTreeRef GetRef(const std::string& refname) const = 0;
};

/**
 * Thrown by GetObject when the object requested is not present in the
 * repository.
 */
class OSTreeObjectMissing : std::exception {
 public:
  explicit OSTreeObjectMissing(const OSTreeHash _missing_object) : missing_object_(_missing_object) {}

  const char* what() const noexcept override { return "OSTree repository is missing an object"; }

  OSTreeHash missing_object() const { return missing_object_; }

 private:
  OSTreeHash missing_object_;
};
// vim: set tabstop=2 shiftwidth=2 expandtab:
#endif  // SOTA_CLIENT_TOOLS_OSTREE_REPO_H_

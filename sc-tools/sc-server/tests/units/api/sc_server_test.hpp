/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/test/sc_test.hpp>

#include <memory>
#include <unordered_set>
#include <filesystem>

#include <sc-memory/sc_memory.hpp>
#include <sc-memory/sc_keynodes.hpp>

#include <sc-builder/scs_loader.hpp>

#include "sc-server-impl/sc_server_impl.hpp"

class ScServerTest : public testing::Test
{
public:
  static inline std::string const & SC_SERVER_INI = "../../sc-server-test.ini";
  static inline std::string const & SC_SERVER_KB_BIN = "sc-server-test-kb-bin";
  static inline std::string const & SC_SERVER_EXTENSIONS = "/extensions";
  static inline std::string const & SC_SERVER_KB = "../../kb";

protected:
  void LoadKB(std::unique_ptr<ScAgentContext> const & m_context, std::unordered_set<std::string> const & sources)
  {
    ScsLoader loader;
    for (std::string const & source : sources)
      loader.loadScsFile(*m_context, ScServerTest::SC_SERVER_KB + "/" + source);
  }

  void SetUp() override
  {
    ScServerTest::Initialize(SC_TRUE);
    m_ctx = std::make_unique<ScAgentContext>();
  }

  void TearDown() override
  {
    if (m_ctx != nullptr)
      m_ctx->Destroy();

    ScServerTest::Shutdown();

    std::filesystem::remove_all(SC_SERVER_KB_BIN);
  }

  void Initialize(sc_bool parallel_actions)
  {
    sc_memory_params params;
    sc_memory_params_clear(&params);

    params.dump_memory = SC_FALSE;
    params.dump_memory_statistics = SC_FALSE;

    params.clear = SC_TRUE;
    params.storage = SC_SERVER_KB_BIN.c_str();

    ScMemory::LogMute();
    ScMemory::Initialize(params);
    m_server = std::make_unique<ScServerImpl>("127.0.0.1", 8898, parallel_actions);
    m_server->ClearChannels();
    m_server->Run();
    ScMemory::LogUnmute();
  }

  void Shutdown()
  {
    ScMemory::LogMute();
    m_server->Stop();
    m_server = nullptr;
    ScMemory::Shutdown();
    ScMemory::LogUnmute();
  }

protected:
  std::unique_ptr<ScAgentContext> m_ctx;
  std::unique_ptr<ScServer> m_server;
};

class ScServerTestWithoutParallelMode : public ScServerTest
{
protected:
  void SetUp() override
  {
    Initialize(SC_FALSE);
    m_ctx = std::make_unique<ScAgentContext>();
  }
};

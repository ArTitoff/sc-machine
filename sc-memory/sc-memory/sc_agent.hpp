/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "sc_object.hpp"

#include "sc_agent_context.hpp"
#include "sc_agent_builder.hpp"

#include "utils/sc_log.hpp"

//! Log functions to be used in agent class methods.
#define SC_AGENT_LOG_DEBUG(__msg__) SC_LOG_DEBUG(GetName() << ": " << __msg__)
#define SC_AGENT_LOG_INFO(__msg__) SC_LOG_INFO(GetName() << ": " << __msg__)
#define SC_AGENT_LOG_WARNING(__msg__) SC_LOG_WARNING(GetName() << ": " << __msg__)
#define SC_AGENT_LOG_ERROR(__msg__) SC_LOG_ERROR(GetName() << ": " << __msg__)

template <class TScEvent>
class TScElementaryEventSubscription;
class ScAction;
class ScResult;

/*!
 * @class ScAgentAbstract
 * @brief An abstract base class for sc-agents.
 *
 * This class provides a base implementation for sc-agents, offering methods for initialization, shutdown, and handling
 * events.
 *
 * @tparam TScEvent The type of sc-event this agent handles.
 */
template <class TScEvent>
class _SC_EXTERN ScAgentAbstract : public ScObject
{
  static_assert(std::is_base_of<class ScEvent, TScEvent>::value, "TScEvent type must be derived from ScEvent type.");

public:
  _SC_EXTERN ~ScAgentAbstract() override;

  /*!
   * @brief Gets abstract agent for agent of this class.
   *
   * This method searches in knowledge base the following construction
   *
   *             nrel_inclusion
   *                  |
   *                  |
   *                  |
   *                  \/
   * abstract_agent ======> agent_implementation
   *
   * and returns `abstract_agent`.
   *
   * @return A sc-address of abstract agent.
   * @throws utils::ExceptionInvalidState if the agent implementation for this agent class is not included in any
   * abstract sc-agent.
   */
  _SC_EXTERN virtual ScAddr GetAbstractAgent() const;

  /*!
   * @brief Gets the sc-event class to which the agent class is subscribed.
   *
   * This method searches in knowledge base the following construction
   *
   *   nrel_primary_initiation_condition
   *                  |
   *                  |
   *                  |  event_class
   *                  \/     ||
   * abstract_agent =======> ||
   *                         ||
   *                         \/
   *               event_subscription_element
   *
   * and returns `event_class`.
   *
   * @return A sc-address of the event class.
   * @throws utils::ExceptionInvalidState if the abstract sc-agent for this agent class does not have a primary
   * initiation condition.
   */
  _SC_EXTERN virtual ScAddr GetEventClass() const;

  /*!
   * @brief Gets sc-event subscription sc-element for which sc-event initiates.
   *
   * This method searches in knowledge base the following construction
   *
   *   nrel_primary_initiation_condition
   *                  |
   *                  |
   *                  |  event_class
   *                  \/     ||
   * abstract_agent =======> ||
   *                         ||
   *                         \/
   *               event_subscription_element
   *
   * and returns `event_subscription_element`.
   *
   * @return A sc-address of sc-event subscription sc-element.
   * @throws utils::ExceptionInvalidState if the abstract sc-agent for this agent class does not have a primary
   * initiation condition.
   */
  _SC_EXTERN virtual ScAddr GetEventSubscriptionElement() const;

  /*!
   * @brief Gets action class that the agent interpreters.
   *
   * This method searches in knowledge base the following construction
   *
   *            nrel_sc_agent_action_class
   *                        |
   *                        |
   *                        |
   *                        \/
   * agent_implementation ======> action_class
   *
   * and returns `action_class`.
   *
   * @return A sc-address of the action class.
   * @throws utils::ExceptionInvalidState if the abstract sc-agent for this agent class does not have an action class.
   */
  _SC_EXTERN virtual ScAddr GetActionClass() const;

  /*!
   * @brief Checks initiation condition for agent of this class.
   * @param event An initiated sc-event to which the agent reacted.
   * @return SC_TRUE if initiation condition was checked successfully, otherwise SC_FALSE.
   */
  _SC_EXTERN virtual sc_bool CheckInitiationCondition(TScEvent const & event);

  /*!
   * @brief Gets initiation condition for agent of this class.
   *
   * This method searches in knowledge base the following construction
   *
   *   nrel_initiation_condition_and_result
   *                  |
   *                  |
   *                  | initiation_condition
   *                  \/       ||
   * abstract_agent =========> ||
   *                           ||
   *                           \/
   *                      result_condition
   *
   * and returns `initiation_condition`.
   *
   * @return A sc-address of initiation condition.
   * @throws utils::ExceptionInvalidState if the abstract sc-agent for this agent class does not have an initiation
   * condition.
   */
  _SC_EXTERN virtual ScAddr GetInitiationCondition() const;

  /*!
   * @brief Gets initiation condition template represented in program.
   * @return A sc-template of initiation condition.
   */
  _SC_EXTERN virtual ScTemplate GetInitiationConditionTemplate() const;

  /*!
   * @brief Executes the program associated with the agent.
   * @param event A sc-event that triggered the agent.
   * @param action A sc-action to be performed by the agent.
   * @return A result of the program execution.
   */
  _SC_EXTERN virtual ScResult DoProgram(TScEvent const & event, ScAction & action) = 0;

  /*!
   * @brief Gets the result of the agent's execution.
   * @param event An initiated sc-event to which the agent reacted.
   * @param action A sc-action that the agent interpreters.
   * @return SC_TRUE if result condition was checked successfully, otherwise SC_FALSE.
   */
  _SC_EXTERN virtual sc_bool CheckResultCondition(TScEvent const & event, ScAction & action);

  /*!
   * @brief Gets result condition for agent of this class.
   *
   * This method searches in knowledge base the following construction
   *
   *   nrel_initiation_condition_and_result
   *                  |
   *                  |
   *                  | initiation_condition
   *                  \/       ||
   * abstract_agent =========> ||
   *                           ||
   *                           \/
   *                      result_condition
   *
   * and returns `result_condition`.
   *
   * @return A sc-address of result condition.
   * @throws utils::ExceptionInvalidState if the abstract sc-agent for this agent class does not have an result
   * condition.
   */
  _SC_EXTERN virtual ScAddr GetResultCondition() const;

  /*!
   * @brief Gets result condition template represented in program.
   * @return A sc-template of result condition.
   */
  _SC_EXTERN virtual ScTemplate GetResultConditionTemplate() const;

protected:
  mutable ScAgentContext m_memoryCtx;
  ScAddr m_agentImplementationAddr;

  static inline std::
      unordered_map<std::string, std::unordered_map<ScAddr, ScElementaryEventSubscription *, ScAddrHashFunc>>
          m_events;

  _SC_EXTERN ScAgentAbstract();

  /*!
   * @brief Sets initiator of the agent.
   * @param userAddr A sc-address of user that initiated this agent.
   */
  _SC_EXTERN void SetInitiator(ScAddr const & userAddr);

  /*!
   * @brief Sets the implementation of the agent of this class.
   * @param agentImplementationAddr A sc-address of the agent implementation.
   */
  _SC_EXTERN void SetImplementation(ScAddr const & agentImplementationAddr);

  /*!
   * @brief Checks if the agent may be specified in knowledge base.
   * @return SC_TRUE if the agent has implementation in knowledge base, otherwise SC_FALSE.
   */
  _SC_EXTERN sc_bool MayBeSpecified() const;

  static _SC_EXTERN std::function<void(TScEvent const &)> GetCallback(ScAddr const & agentImplementationAddr);
};

/*!
 * @class ScAgent
 * @brief A class for sc-agents that can subscribe to any elementary sc-events.
 *
 * This class extends ScAgentAbstract and provides methods for subscribing and unsubscribing
 * to sc-events.
 *
 * @tparam TScEvent The type of sc-event this agent class handles.
 */
template <class TScEvent>
class _SC_EXTERN ScAgent : public ScAgentAbstract<TScEvent>
{
  static_assert(std::is_base_of<ScEvent, TScEvent>::value, "TScEvent type must be derived from ScEvent type.");

public:
  /*!
   * @brief Subscribes agent class to specified sc-events.
   *
   * If provided agent implementation `agentImplementationAddr` is valid then this method searches in knowledge base the
   * following constructions
   *
   *             nrel_inclusion
   *                  |
   *                  |
   *                  |
   *                  \/
   * abstract_agent ======> agent_implementation
   *
   *   nrel_primary_initiation_condition
   *                  |
   *                  |
   *                  |  event_class
   *                  \/     ||
   * abstract_agent =======> ||
   *                         ||
   *                         \/
   *               event_subscription_element
   *
   * and subscribes the agent of this class to sc-event `event_class` with subscription sc-element
   * `event_subscription_element`, else subcribes the agent of this class to sc-event
   * `TScEvent` with subscription sc-elements from `subscriptionAddrs`.
   *
   * @tparam TScAgent An agent class to be subscribed to the event.
   * @param ctx A sc-memory context used to subscribe agent class to specified sc-event.
   * @param agentImplementationAddr A sc-address of agent implementation specified in knowledge base for this agent
   * class.
   * @param subscriptionAddrs A list of sc-addresses of sc-elements to subscribe to.
   * @warning Specified agent class must be derived from class `ScAgent`.
   * @throws utils::ExceptionInvalidParams if any of the subscription addresses are invalid.
   * @throws utils::ExceptionInvalidState if the agent is already subscribed to the event.
   * @throws utils::ExceptionInvalidState if the agent implementation for this agent class is valid and is not included
   * in any abstract sc-agent.
   * @throws utils::ExceptionInvalidState if the agent implementation for this agent class is valid and the abstract
   * sc-agent for this agent class does not have a primary initiation condition.
   */
  template <class TScAgent, class... TScAddr>
  static _SC_EXTERN void Subscribe(
      ScMemoryContext * ctx,
      ScAddr const & agentImplementationAddr,
      TScAddr const &... subscriptionAddrs);

  /*!
   * @brief Unsubscribes agent class from specified sc-events.
   *
   * If provided agent implementation `agentImplementationAddr` is valid then this method searches in knowledge base the
   * following constructions
   *
   *             nrel_inclusion
   *                  |
   *                  |
   *                  |
   *                  \/
   * abstract_agent ======> agent_implementation
   *
   *   nrel_primary_initiation_condition
   *                  |
   *                  |
   *                  |  event_class
   *                  \/     ||
   * abstract_agent =======> ||
   *                         ||
   *                         \/
   *               event_subscription_element
   *
   * and unsubscribes the agent of this class from sc-event `event_class` with subscription sc-element
   * `event_subscription_element`, else unsubcribes the agent of this class from sc-event
   * `TScEvent` with subscription sc-elements from `subscriptionAddrs`.
   *
   * @tparam TScAgent An agent class to be unsubscribed from the event.
   * @param ctx A sc-memory context used to unsubscribe agent class from specified sc-event.
   * @param agentImplementationAddr A sc-address of agent implementation specified in knowledge base for this agent
   * class.
   * @param subscriptionAddrs A list of sc-addresses of sc-elements to unsubscribe from.
   * @warning Specified agent class must be derived from class `ScAgent`.
   * @throws utils::ExceptionInvalidParams if any of the subscription addresses are invalid.
   * @throws utils::ExceptionInvalidState if the agent is not subscribed to the event.
   * @throws utils::ExceptionInvalidState if the agent implementation for this agent class is valid and is not included
   * in any abstract sc-agent.
   * @throws utils::ExceptionInvalidState if the agent implementation for this agent class is valid and the abstract
   * sc-agent for this agent class does not have a primary initiation condition.
   */
  template <class TScAgent, class... TScAddr>
  static _SC_EXTERN void Unsubscribe(
      ScMemoryContext * ctx,
      ScAddr const & agentImplementationAddr,
      TScAddr const &... subscriptionAddrs);

protected:
  _SC_EXTERN explicit ScAgent();

  /*!
   * @brief Gets the callback function for agent class.
   * @tparam TScAgent An agent class to be subscribed to the event.
   * @param agentImplementationAddr A sc-address of agent implementation specified in knowledge base for this agent
   * class.
   * @return A function that takes an sc-event and returns an sc-result.
   * @warning Specified agent class must be derivied from class `ScAgent`.
   */
  template <class TScAgent>
  static _SC_EXTERN std::function<void(TScEvent const &)> GetCallback(ScAddr const & agentImplementationAddr);

private:
  /*!
   * @brief Builds a check template for the given sc-event and template sc-address.
   * @param event A sc-event to build the template for.
   * @param checkTemplateAddr Ф sc-address of the check template.
   * @return A built sc-template.
   */
  ScTemplate BuildCheckTemplate(TScEvent const & event, ScAddr const & checkTemplateAddr);
};

/*!
 * @class ScActionAgent
 * @brief A specialized agent class for handling sc-actions.
 *
 * This class extends ScAgent and provides methods for subscribing and unsubscribing
 * to sc-action events. You can derive this class and implement your own agent class.
 *
 * @warning Derived classes must override the methods: `GetActionClass` and `DoProgram`.
 *
 * @code
 * // File my_agent.hpp:
 * #pragma once
 *
 * #include <sc-memory/sc_agent.hpp>
 *
 * class MyAgent : public ScActionAgent
 * {
 * public:
 *   ScAddr GetActionClass() const override;
 *
 *   ScResult DoProgram(ScActionEvent const & event, ScAction & action) override;
 * };
 *
 * // File my_agent.cpp:
 * #include "my-module/agents/my_agent.hpp"
 * #include "my-module/keynodes/my_keynodes.hpp"
 *
 * ScAddr MyAgent::GetActionClass() const
 * {
 *   return MyKeynodes::my_agent_action;
 * }
 *
 * ScResult MyAgent::DoProgram(ScActionEvent const &, ScAction & action)
 * {
 *   auto const & [argAddr1, argAddr2] = action.GetArguments<2>();
 *   if (!argAddr1.IsValid() || !argAddr2.IsValid())
 *     return action.FinishWithError();
 *
 *   // Write agent logic here.
 *
 *   action.SetAnswer(answerAddr);
 *   return action.FinishSuccessfully();
 * }
 * @endcode
 */
class _SC_EXTERN ScActionAgent : public ScAgent<ScActionEvent>
{
public:
  /*!
   * @brief Subscribes agent class to sc-event of adding output arc from `action_initiated` to some formed sc-action.
   * @tparam TScAgent An agent class to be subscribed to the event.
   * @param ctx A sc-memory context used to subcribe agent class to specified sc-event.
   * @param agentImplementationAddr A sc-address of agent implementation specified in knowledge base for this agent
   * class.
   * @warning Specified agent class must be derivied from class `ScAgentAction`.
   * @throws utils::ExceptionInvalidState if the agent is already subscribed to the event.
   */
  template <class TScAgent>
  static _SC_EXTERN void Subscribe(ScMemoryContext * ctx, ScAddr const & agentImplementationAddr);

  /*!
   * @brief Unsubscribes agent class from sc-event of adding output arc from `action_initiated` to some formed
   * sc-action.
   * @tparam TScAgent An agent class to be subscribed to the event.
   * @param ctx A sc-memory context used to unsubcribe agent class from specified sc-event.
   * @param agentImplementationAddr A sc-address of agent implementation specified in knowledge base for this agent
   * class.
   * @warning Specified agent class must be derivied from class `ScAgentAction`.
   * @throws utils::ExceptionInvalidState if the agent is not subscribed to the event.
   */
  template <class TScAgent>
  static _SC_EXTERN void Unsubscribe(ScMemoryContext * ctx, ScAddr const & agentImplementationAddr);

  /*!
   * @brief Get sc-template that other sc-element of initiated sc-event belongs to action class that this agent class
   * interpreters.
   * @return ScTemplate of initiation condition of this agent class.
   */
  _SC_EXTERN ScTemplate GetInitiationConditionTemplate() const override;

protected:
  ScActionAgent();
};

#include "sc_agent.tpp"

/*!
 * @brief Subscribes agent class to specified sc-events.
 * @tparam TScAgent An agent class to be subscribed to the event.
 * @param ctx A sc-memory context used to subcribe agent class to specified sc-event.
 * @param subscriptionAddrs A list of sc-addresses of sc-elements to subscribe to.
 * @warning Specified agent class must be derivied from class `ScAgent`.
 * @throws utils::ExceptionInvalidParams if any of the subscription addresses are invalid.
 * @throws utils::ExceptionInvalidState if the agent is already subscribed to the event.
 */
template <class TScAgent, class... TScAddr>
typename std::enable_if<!std::is_base_of<ScActionAgent, TScAgent>::value>::type SubscribeAgent(
    ScMemoryContext * ctx,
    TScAddr const &... subscriptionAddrs)
{
  static_assert(
      (std::is_base_of<ScAddr, TScAddr>::value && ...), "Each element of parameter pack must have ScAddr type.");

  TScAgent::template Subscribe<TScAgent>(ctx, ScAddr::Empty, subscriptionAddrs...);
}

/*!
 * @brief Subscribes agent class to sc-event of adding output arc from `action_initiated` to some formed sc-action.
 * @tparam TScAgent An agent class to be subscribed to the event.
 * @param ctx A sc-memory context used to subcribe agent class to specified sc-event.
 * @warning Specified agent class must be derivied from class `ScAgentAction`.
 * @throws utils::ExceptionInvalidParams if any of the subscription addresses are invalid.
 * @throws utils::ExceptionInvalidState if the agent is already subscribed to the event.
 */
template <class TScAgent>
typename std::enable_if<std::is_base_of<ScActionAgent, TScAgent>::value>::type SubscribeAgent(ScMemoryContext * ctx)
{
  ScActionAgent::template Subscribe<TScAgent>(ctx, ScAddr::Empty);
}

/*!
 * @brief Unsubscribes agent class from specified sc-events.
 * @tparam TScAgent An agent class to be subscribed to the event.
 * @param ctx A sc-memory context used to unsubcribe agent class from specified sc-event.
 * @param subscriptionAddrs A list of sc-addresses of sc-elements to subscribe from.
 * @warning Specified agent class must be derivied from class `ScAgent`.
 *  @throws utils::ExceptionInvalidParams if any of the subscription addresses are invalid.
 * @throws utils::ExceptionInvalidState if the agent is not subscribed to the event.
 */
template <class TScAgent, class... TScAddr>
typename std::enable_if<!std::is_base_of<ScActionAgent, TScAgent>::value>::type UnsubscribeAgent(
    ScMemoryContext * ctx,
    TScAddr const &... subscriptionAddrs)
{
  static_assert(
      (std::is_base_of<ScAddr, TScAddr>::value && ...), "Each element of parameter pack must have ScAddr type.");

  TScAgent::template Unsubscribe<TScAgent>(ctx, ScAddr::Empty, subscriptionAddrs...);
}

/*!
 * @brief Unsubscribes agent class from sc-event of adding output arc from `action_initiated` to some formed
 * sc-action.
 * @tparam TScAgent An agent class to be subscribed to the event.
 * @param ctx A sc-memory context used to unsubcribe agent class from specified sc-event.
 * @warning Specified agent class must be derivied from class `ScAgentAction`.
 * @throws utils::ExceptionInvalidParams if any of the subscription addresses are invalid.
 * @throws utils::ExceptionInvalidState if the agent is not subscribed to the event.
 */
template <class TScAgent>
typename std::enable_if<std::is_base_of<ScActionAgent, TScAgent>::value>::type UnsubscribeAgent(ScMemoryContext * ctx)
{
  ScActionAgent::template Unsubscribe<TScAgent>(ctx, ScAddr::Empty);
}

/*!
 * @brief Builds and subscribes an agent.
 * @tparam TScAgent An agent class to be subscribed to the event.
 * @param ctx A sc-memory context used to build and subscribe agent.
 * @param agentImplementationAddr A sc-address of agent implementation specified in knowledge base for this agent class.
 * @throws utils::ExceptionInvalidParams if any of the subscription addresses are invalid.
 * @throws utils::ExceptionInvalidState if the agent is already subscribed to the event.
 * @throws utils::ExceptionInvalidState if the agent implementation for this agent class is valid and is not included
 * in any abstract sc-agent.
 * @throws utils::ExceptionInvalidState if the agent implementation for this agent class is valid and the abstract
 * sc-agent for this agent class does not have a primary initiation condition.
 */
template <class TScAgent>
void BuildAndSubscribeAgent(ScMemoryContext * ctx, ScAddr const & agentImplementationAddr)
{
  ScAgentBuilder<TScAgent> builder{agentImplementationAddr};
  builder.LoadSpecification(ctx);

  TScAgent::template Subscribe<TScAgent>(ctx, agentImplementationAddr);
}

/*!
 * @brief Destroys and unsubscribes an agent.
 * @tparam TScAgent An agent class to be unsubscribed from the event.
 * @param ctx A sc-memory context used to destroy and unsubscribe agent.
 * @param agentImplementationAddr A sc-address of agent implementation specified in knowledge base for this agent class.
 *  @throws utils::ExceptionInvalidParams if any of the subscription addresses are invalid.
 * @throws utils::ExceptionInvalidState if the agent is not subscribed to the event.
 * @throws utils::ExceptionInvalidState if the agent implementation for this agent class is valid and is not included
 * in any abstract sc-agent.
 * @throws utils::ExceptionInvalidState if the agent implementation for this agent class is valid and the abstract
 * sc-agent for this agent class does not have a primary initiation condition.
 */
template <class TScAgent>
void DestroyAndUnsubscribeAgent(ScMemoryContext * ctx, ScAddr const & agentImplementationAddr)
{
  TScAgent::template Unsubscribe<TScAgent>(ctx, agentImplementationAddr);
}

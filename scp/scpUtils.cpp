/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "scpUtils.hpp"
#include "scpKeynodes.hpp"

#include <string>
#include <iostream>

using namespace std;

namespace scp {
namespace Utils {

bool addToSet(const std::unique_ptr<ScMemoryContext>& ctx, ScAddr const& setAddr, ScAddr const& elAddr)
{
    if (ctx->HelperCheckArc(setAddr, elAddr, ScType::EdgeAccessConstPosPerm))
        return false;

    ScAddr arcAddr = ctx->CreateEdge(ScType::EdgeAccessConstPosPerm, setAddr, elAddr);
    assert(arcAddr.IsValid());
    return true;
}

bool removeFromSet(const std::unique_ptr<ScMemoryContext>& ctx, ScAddr const& setAddr, ScAddr const& elAddr)
{
    ScIterator3Ptr it = ctx->Iterator3(setAddr, ScType::EdgeAccessConstPosPerm, elAddr);
    bool result = false;
    while (it->Next())
        ctx->EraseElement(it->Get(1));

    return result;
}

bool resolveOrderRoleRelation(const std::unique_ptr<ScMemoryContext>& ctx, ScAddr const& arcAddr, ScAddr& relationAddr)
{
    ScIterator3Ptr it = ctx->Iterator3(ScType::NodeConst, ScType::EdgeAccess, arcAddr);
    while (it->Next())
    {
        if (ctx->HelperCheckArc(Keynodes::order_role_relation, it->Get(0), ScType::EdgeAccessConstPosPerm))
        {
            relationAddr = it->Get(0);
            return true;
        }
    }

    return false;
}

bool resolveOrderRoleRelation(const std::unique_ptr<ScMemoryContext>& ctx, uint8_t const order, ScAddr& relationAddr)
{
    switch (order)
    {
        case 1:
            relationAddr = Keynodes::rrel_1;
            return true;
            break;
        case 2:
            relationAddr = Keynodes::rrel_2;
            return true;
            break;
        case 3:
            relationAddr = Keynodes::rrel_3;
            return true;
            break;
        case 4:
            relationAddr = Keynodes::rrel_4;
            return true;
            break;
        case 5:
            relationAddr = Keynodes::rrel_5;
            return true;
            break;
        case 6:
            relationAddr = Keynodes::rrel_6;
            return true;
            break;
        case 7:
            relationAddr = Keynodes::rrel_7;
            return true;
            break;
        case 8:
            relationAddr = Keynodes::rrel_8;
            return true;
            break;
        case 9:
            relationAddr = Keynodes::rrel_9;
            return true;
            break;
        case 10:
            relationAddr = Keynodes::rrel_10;
            return true;
            break;
        default:
            relationAddr.Reset();
            return false;
            break;
    }
}

bool resolveOperatorType(const std::unique_ptr<ScMemoryContext>& ctx, ScAddr const& operatorAddr, ScAddr& operatorType)
{
    ScIterator3Ptr it = ctx->Iterator3(ScType::NodeConst, ScType::EdgeAccess, operatorAddr);
    while (it->Next())
    {
        if (ctx->HelperCheckArc(Keynodes::scp_operator_atomic_type, it->Get(0), ScType::EdgeAccessConstPosPerm))
        {
            operatorType = it->Get(0);
            return true;
        }
    }

    return false;
}

void printSystemIdentifier(const std::unique_ptr<ScMemoryContext>& ctx, ScAddr const& elemAddr)
{
    if (ctx->GetElementType(elemAddr).IsNode() || ctx->GetElementType(elemAddr).IsLink())
    {
        string s = ctx->HelperGetSystemIdtf(elemAddr);
        if (s == "")
            cout << elemAddr.GetRealAddr().seg << "|" << elemAddr.GetRealAddr().offset;
        else
            cout << s;
    }
    else
    {
        cout << "(";
        printSystemIdentifier(ctx, ctx->GetArcBegin(elemAddr));
        cout << "->";
        printSystemIdentifier(ctx, ctx->GetArcEnd(elemAddr));
        cout << ")";
    }
}

void printInfo(const std::unique_ptr<ScMemoryContext>& ctx, ScAddr const& elemAddr)
{
    int c_in = 0, c_out = 0;
    assert(ctx->IsElement(elemAddr));
    printSystemIdentifier(ctx, elemAddr);
    cout << endl;;

    cout << "Input arcs:\n";
    ScIterator3Ptr it = ctx->Iterator3(ScType(0), ScType(0), elemAddr);
    while (it->Next())
    {
        c_in++;
        cout << "\t" << it->Get(1).GetRealAddr().seg << "|" << it->Get(1).GetRealAddr().offset;
        if (ctx->GetElementType(it->Get(1)).BitAnd(sc_type_arc_access))
            cout << " <- ";
        else
            cout << " <= ";
        printSystemIdentifier(ctx, it->Get(0));
        cout << endl;
    }
    cout << "Total input arcs: " << c_in << endl;

    cout << "Output arcs:\n";
    it = ctx->Iterator3(elemAddr, ScType(0), ScType(0));
    while (it->Next())
    {
        c_out++;
        cout << "\t" << it->Get(1).GetRealAddr().seg << "|" << it->Get(1).GetRealAddr().offset;
        if (ctx->GetElementType(it->Get(1)).BitAnd(sc_type_arc_access))
            cout << " -> ";
        else
            cout << " => ";
        printSystemIdentifier(ctx, it->Get(2));
        cout << endl;
    }
    cout << "Total output arcs: " << c_out << endl;
}

#ifdef SCP_DEBUG
void logUnknownOperatorTypeError(const std::unique_ptr<ScMemoryContext>& ctx, ScAddr& addr)
{
    cout << "SCP Error: Unknown scp-operator type: ";
    printSystemIdentifier(ctx, addr);
    cout << endl;
}

void logSCPError(const std::unique_ptr<ScMemoryContext>& ctx, string text, ScAddr& addr)
{
    cout << "SCP Error: " << text << ": ";
    printSystemIdentifier(ctx, addr);
    cout << endl;
}
#endif

}
}

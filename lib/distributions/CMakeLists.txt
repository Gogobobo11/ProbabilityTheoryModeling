add_library(distributions STATIC
        NormalDistribution.cpp
        UniformDistribution.cpp
        ExponentialDistribution.cpp
        CauchyDistribution.cpp
        LaplaceDistribution.cpp
        BernoulliDistribution.cpp
        BinomialDistribution.cpp
        GeometricDistribution.cpp
        PoissonDistribution.cpp
        LogisticDistribution.cpp
        MaxwellDistribution.cpp
        DistributionExperiment.cpp
        ../markov-chain/utility/SplitText.hpp
)

target_include_directories(distributions PUBLIC ${PROJECT_SOURCE_DIR}/lib)
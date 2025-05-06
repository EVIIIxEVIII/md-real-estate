#pragma once

#include <Eigen/Dense>
#include <csv.h>

namespace price_model {

struct SplitData {
    Eigen::MatrixXd X_train;
    Eigen::VectorXd Y_train;

    Eigen::MatrixXd x_test;
    Eigen::VectorXd y_test;
};

template<std::size_t F>
struct LoadedData {
    std::vector<std::array<double, F>> X_rows;
    std::vector<double>  y_vals;
};

class DataHandler {
    public:
        DataHandler(std::string data_path) {
            load_data(data_path);
        };

        static constexpr std::array feature {
            //"total_area","number_of_rooms","number_of_floors","bathroom","floor",
            //"is_penthouse","apartment_condition_Casă de demolat","apartment_condition_Construcție nefinisată","apartment_condition_Dat în exploatare","apartment_condition_Design individual","apartment_condition_Euroreparație","apartment_condition_Fără reparație","apartment_condition_Reparație cosmetică","apartment_condition_Variantă albă","apartment_condition_Variantă sură","housing_fund_Secundar","region_Bacioi_Centru","region_Bacioi_Periferie","region_Basarabeasca_Centru","region_Bender/Tighina_Balca","region_Bender/Tighina_Bam","region_Bender/Tighina_Centru","region_Bender/Tighina_Microraionul Lenin","region_Bender/Tighina_Microraionul de Nord","region_Bender/Tighina_Microraionul Şelkovâi","region_Biruința_Centru","region_Briceni_Centru","region_Briceni_Periferie","region_Bubuieci_Centru","region_Bubuieci_Periferie","region_Budești_Centru","region_Bugeac_Centru","region_Bugeac_Periferie","region_Bulboaca_Centru","region_Bălți_10 cartier","region_Bălți_5 cartier","region_Bălți_6 cartier","region_Bălți_7 cartier","region_Bălți_8 cartier","region_Bălți_9 cartier","region_Bălți_Autogara","region_Bălți_BAM","region_Bălți_Centru","region_Bălți_Dvoreanskoe gnezdo","region_Bălți_Gara de nord","region_Bălți_Molodova","region_Bălți_Paminteni","region_Bălți_Podul Chișinăului","region_Bălți_Pole ciudes","region_Bălți_Steluța","region_Cahul_Centru","region_Cahul_Periferie","region_Ceadîr-Lunga_Centru","region_Ceadîr-Lunga_Periferie","region_Chetrosu_Centru","region_Chișinău_Aeroport","region_Chișinău_Botanica","region_Chișinău_Buiucani","region_Chișinău_Centru","region_Chișinău_Ciocana","region_Chișinău_Durlești","region_Chișinău_Poșta Veche","region_Chișinău_Râșcani","region_Chișinău_Sculeni","region_Chișinău_Telecentru","region_Cimișlia_Centru","region_Ciorescu_Centru","region_Ciorescu_Periferie","region_Codru_Centru","region_Codru_Periferie","region_Cojușna_Centru","region_Cojușna_Periferie","region_Colonița_Centru","region_Colonița_Periferie","region_Comrat_Centru","region_Comrat_Periferie","region_Congaz_Centru","region_Cornești_Centru","region_Costești_Centru","region_Cricova_Centru","region_Cricova_Periferie","region_Criuleni_Centru","region_Cupcini_Centru","region_Căinari_Centru","region_Călăraşi_Centru","region_Călăraşi_Periferie","region_Căușeni_Centru","region_Căușeni_Periferie","region_Dobrogea_Centru","region_Dondușeni_Centru","region_Drochia_Centru","region_Drochia_Periferie","region_Dubăsari_Centru","region_Dumbrava_Centru","region_Dumbrava_Periferie","region_Dănceni_Centru","region_Edineț_Centru","region_Edineț_Periferie","region_Elizaveta_Periferie","region_Floreni_Centru","region_Floreni_Periferie","region_Florești_Centru","region_Florești_Periferie","region_Fălești_Centru","region_Fălești_Periferie","region_Ghidighici_Centru","region_Glodeni_Centru","region_Glodeni_Periferie","region_Grigoriopol_Centru","region_Grătiești_Centru","region_Gălești_Centru","region_Hîncești_Centru","region_Hîncești_Periferie","region_Hîrjauca_Centru","region_Ialoveni_Centru","region_Ialoveni_Periferie","region_Leova_Centru","region_Leova_Periferie","region_Merenii Noi_Centru","region_Micăuți_Centru","region_Nisporeni_Centru","region_Ocnița_Centru","region_Ocnița_Periferie","region_Orhei_Centru","region_Orhei_Periferie","region_Otaci_Centru","region_Pelivan_Centru","region_Peresecina_Centru","region_Piatra Albă_Centru","region_Piatra Albă_Periferie","region_Porumbeni_Centru","region_Rezina_Centru","region_Rîbnița_Centru","region_Rîșcani_Centru","region_Rîșcani_Periferie","region_Soroca_Centru","region_Soroca_Periferie","region_Speia_Centru","region_Stejăreni_Centru","region_Strășeni_Centru","region_Strășeni_Periferie","region_Stăuceni_Centru","region_Svetlîi_Centru","region_Sîngera_Centru","region_Sîngera_Periferie","region_Sîngerei_Centru","region_Sîngerei_Periferie","region_Taraclia_Centru","region_Taraclia_Periferie","region_Telenești_Centru","region_Tiraspol_Borodinka","region_Tiraspol_Centru","region_Tiraspol_Fedico","region_Tiraspol_Mecinikov","region_Tiraspol_Microraionul de Vest","region_Tohatin_Centru","region_Trușeni_Centru","region_Trușeni_Periferie","region_Ungheni_Bereşti","region_Ungheni_Centru","region_Ungheni_Dănuțeni","region_Ungheni_Tineret","region_Vadul lui Vodă_Periferie","region_Varnita_Centru","region_Vatra_Centru","region_Vatra_Periferie","region_Vișniovca_Centru","region_Vulcănești_Centru","region_Șoldănești_Centru","region_Țînțăreni_Centru","living_room_Apartament fără living","parking_spot_Garaj","parking_spot_Sub acoperiș","parking_spot_Subterană","compartmentalization_135","compartmentalization_143","compartmentalization_Brj (Brejnevka)","compartmentalization_Cămin familial","compartmentalization_Cş (Ceşka)","compartmentalization_Garsonieră","compartmentalization_Hruşciovka","compartmentalization_Individuală","compartmentalization_Ms (serie  moldovenească)","compartmentalization_Pe pămănt","compartmentalization_Rubaşka","compartmentalization_St (Stalinka)","compartmentalization_Varț (Varnițkaia)","compartmentalization_Городской","compartmentalization_Итальянский Двор","compartmentalization_Московский"

            "total_area","number_of_rooms","number_of_floors","bathroom","floor","apartment_condition","housing_fund","region","living_room","parking_spot","is_penthouse","compartmentalization"

            // "MSSubClass","MSZoning","LotFrontage","LotArea","Street","Alley","LotShape","LandContour","Utilities","LotConfig","LandSlope","Neighborhood","Condition1","Condition2","BldgType","HouseStyle","OverallQual","OverallCond","YearBuilt","YearRemodAdd","RoofStyle","RoofMatl","Exterior1st","Exterior2nd","MasVnrType","MasVnrArea","ExterQual","ExterCond","Foundation","BsmtQual","BsmtCond","BsmtExposure","BsmtFinType1","BsmtFinSF1","BsmtFinType2","BsmtFinSF2","BsmtUnfSF","TotalBsmtSF","Heating","HeatingQC","CentralAir","Electrical","1stFlrSF","2ndFlrSF","LowQualFinSF","GrLivArea","BsmtFullBath","BsmtHalfBath","FullBath","HalfBath","BedroomAbvGr","KitchenAbvGr","KitchenQual","TotRmsAbvGrd","Functional","Fireplaces","FireplaceQu","GarageType","GarageYrBlt","GarageFinish","GarageCars","GarageArea","GarageQual","GarageCond","PavedDrive","WoodDeckSF","OpenPorchSF","EnclosedPorch","3SsnPorch","ScreenPorch","PoolArea","PoolQC","Fence","MiscFeature","MiscVal","MoSold","YrSold","SaleType","SaleCondition"
        };

        static constexpr std::size_t F = feature.size();

        SplitData get_split_data(float training_ratio);

    private:
        LoadedData<F> loaded_data;
        void load_data(std::string data_path);
        template<std::size_t... Is>
        bool read_row_array(io::CSVReader<F + 1>& in,
                            std::array<double, F>&  x,
                            double&                 y,
                            std::index_sequence<Is...>);
};

}

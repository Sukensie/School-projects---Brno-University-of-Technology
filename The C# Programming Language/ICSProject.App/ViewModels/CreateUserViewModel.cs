using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Input;
using System.Threading.Tasks;
using ICSProject.App.ViewModels;
using ICSProject.BL.Models;
using ICSProject.App.Commands;
using ICSProject.BL.Facades;
using System.Windows;
using System.Threading.Tasks;
using ICSProject.App.Services;

namespace ICSProject.App.ViewModels
{
    public class CreateUserViewModel : ViewModelBase, ICreateUserViewModel
    {
        private readonly UserFacade _userFacade;
        private readonly IMessageDialogService _messageDialogService;

        public ICommand CreateUserCommand { get; }

        private string _userName = string.Empty;
        private string _userEmail = string.Empty;
        private string _userTelephoneNumber = string.Empty;
        private string _userHometown = string.Empty;
        private string _userImageUrl = string.Empty;
        private DateTime _userBirthday;


        public string UserName
        {
            get
            {
                return _userName;
            }
            set
            {
                _userName = value;
                OnPropertyChanged(nameof(UserName));
            }
        }
        public string UserEmail
        {
            get
            {
                return _userEmail;
            }
            set
            {
                _userEmail = value;
                OnPropertyChanged(nameof(UserEmail));
            }
        }
        public string UserTelephoneNumber
        {
            get
            {
                return _userTelephoneNumber;
            }
            set
            {
                _userTelephoneNumber = value;
                OnPropertyChanged(nameof(UserTelephoneNumber));
            }
        }
        public string UserHometown
        {
            get
            {
                return _userHometown;
            }
            set
            {
                _userHometown = value;
                OnPropertyChanged(nameof(UserHometown));
            }
        }
        public string UserImageUrl
        {
            get
            {
                return _userImageUrl;
            }
            set
            {
                _userImageUrl = value;
                OnPropertyChanged(nameof(UserImageUrl));
            }
        }
        public DateTime UserBirthday
        {
            get
            {
                return _userBirthday;
            }
            set
            {
                _userBirthday = value;
                OnPropertyChanged(nameof(UserBirthday));
            }
        }


        private Visibility visibility = Visibility.Hidden;
        public Visibility Visibility
        {
            get
            {
                return visibility;
            }
            set
            {
                visibility = value;

                OnPropertyChanged("Visibility");
            }
        }
        

        public CreateUserViewModel(
            MenuViewModel menuViewModel,
            UserFacade userFacade,
            IMessageDialogService messageDialogService
        )
        {
            _userFacade = userFacade;
            MenuViewModel = menuViewModel;
            CreateUserCommand = new RelayCommand<UserDetailModel>(UserCreated);
            _messageDialogService = messageDialogService;
        }

        //todo prepracovat na async?
        private void UserCreated(UserDetailModel? userDetailModel)
        {
            //nevytvářej nového uživatele, pokud je nějaký z povinných údajů nevyplněný
            if (UserName == string.Empty || UserEmail == string.Empty || UserTelephoneNumber == string.Empty ||
                UserHometown == string.Empty)
            {
                var __ = _messageDialogService.Show(
                    "Chyba!",
                    "Pro vytvoření uživatele musí být všecha políčka (vyjma URL profilového obrázku) vyplněna!",
                    MessageDialogButtonConfiguration.OK,
                    MessageDialogResult.OK);
                return;
            }

            if (UserImageUrl == string.Empty)
            {
                UserImageUrl = "/Resources/Images/DefaultProfile.png";
            }

            var newUser = new UserDetailModel
            (
                Name: UserName,
                Email: UserEmail,
                TelephoneNumber: UserTelephoneNumber,
                Birthday: UserBirthday,
                Hometown: UserHometown,
                ImageUrl: UserImageUrl
            );

            _userFacade.SaveAsync(newUser);
            
            
            Visibility = Visibility.Visible;
           
            

            //vynulování textblocků po přidání nového uživatele
            UserName = string.Empty;
            UserEmail = string.Empty;
            UserTelephoneNumber = string.Empty;
            UserHometown = string.Empty;
            UserImageUrl = string.Empty;
            UserBirthday = DateTime.MinValue;

            var _ = _messageDialogService.Show(
                "Přidání uživatele",
                "Nový uživatel úspěšně vytvořen!",
                MessageDialogButtonConfiguration.OK,
                MessageDialogResult.OK);
        }

        public MenuViewModel MenuViewModel { get; set; }
    }
}

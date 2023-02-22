using System.Windows;
using System.Windows.Input;

namespace ICSProject.App.Controls
{
    public partial class ButtonIcon
    {

        public ICommand Cmd
        {
            get => (ICommand)GetValue(CmdProperty);
            set => SetValue(CmdProperty, value); 
        }

        public string Src
        {
            get => (string)GetValue(SrcProperty);
            set => SetValue(SrcProperty, value); 
        }

        public string Txt
        {
            get => (string)GetValue(TxtProperty);
            set => SetValue(TxtProperty, value); 
        }

        public Style ButtonStyle
        {
            get => (Style)GetValue(ButtonStyleProperty);
            set => SetValue(ButtonStyleProperty, value);
        }


        public static readonly DependencyProperty TxtProperty = DependencyProperty.Register(
            nameof(Txt),
            typeof(string),
            typeof(ButtonIcon),
            new PropertyMetadata(default(string)));

        public static readonly DependencyProperty SrcProperty = DependencyProperty.Register(
            nameof(Src),
            typeof(string),
            typeof(ButtonIcon),
            new PropertyMetadata(default(string)));

        public static readonly DependencyProperty ButtonStyleProperty = DependencyProperty.Register(
            nameof(ButtonStyle),
            typeof(Style),
            typeof(ButtonIcon),
            new PropertyMetadata(default(Style)));

        public static readonly DependencyProperty CmdProperty = DependencyProperty.Register(
            nameof(Cmd),
            typeof(ICommand),
            typeof(ButtonIcon),
            new PropertyMetadata(default(ICommand)));

        public ButtonIcon()
        {
            InitializeComponent();
        }
    }
}
